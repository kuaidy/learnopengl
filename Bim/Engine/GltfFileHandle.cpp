#include "GltfFileHandle.h"
#include <string>
#include "Utility.h"
#include "../Graphics/Point.h"
#include "../json.hpp"
#include "../Graphics/Geometry.h"

namespace Bim
{
	namespace Engine
	{
		GltfFileHandle::GltfFileHandle() {

		}

		bool GltfFileHandle::ReadFile(const std::filesystem::path& filepath) {
			tinygltf::Model model;
			tinygltf::TinyGLTF loader;
			std::string err, warn;
			std::string fileExt = filepath.extension().string();
			bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filepath.string());
			if (!ret) {
				ret = loader.LoadBinaryFromFile(&model, &err, &warn, filepath.string());
				if (!ret) {
					return false;
				}
			}
			_model = model;
			GetNodeMatrix(model);

			const tinygltf::Scene& scene = model.scenes[model.defaultScene];

			auto& ctx = Engine::GetGlobalContext();
			ctx.scene = std::make_shared<Scene::Node>();

			for (int rootNode : scene.nodes) {
				AnalysisModel(model, rootNode, ctx.scene);
			}
			return true;
		}

		/// <summary>
		/// 从二进制解析模型
		/// </summary>
		/// <param name="data"></param>
		/// <returns></returns>
		bool GltfFileHandle::ReadFile(const unsigned char* data, size_t size) {
			tinygltf::Model model;
			tinygltf::TinyGLTF loader;
			std::string err, warn;
			/*std::ofstream file("C:/Users/4513/Desktop/test1.glb", std::ios::binary);
			file.write(reinterpret_cast<const char*>(data), size);
			file.close();*/
			bool ret = loader.LoadBinaryFromMemory(&model, &err, &warn, data, size);
			if (!ret) {
				return false;
			}
			_model = model;
			GetNodeMatrix(model);
			const tinygltf::Scene& scene = model.scenes[model.defaultScene];

			auto ctx = Engine::GetGlobalContext();
			ctx.scene = std::make_shared<Scene::Node>();

			for (int rootNode : scene.nodes) {
				AnalysisModel(model, rootNode, ctx.scene);
			}
			return true;
		}
		bool GltfFileHandle::WriteFile(const std::filesystem::path& filepath) {
			return true;
		}
		/// <summary>
		/// 解析模型
		/// </summary>
		/// <param name="model"></param>
		void GltfFileHandle::AnalysisModel(const tinygltf::Model& model, int nodeIndex, const std::shared_ptr<Scene::Node>& parentNode) {
			auto node = model.nodes[nodeIndex];
			std::shared_ptr<Scene::Node> sceneNode = std::make_shared<Scene::Node>();
			sceneNode->guid = node.name;
			sceneNode->localMatrix = GetLocalMatrix(node);
			sceneNode->globalMatrix = _worldMatrices[nodeIndex];

			if (node.mesh >= 0) {
				tinygltf::Mesh mesh = model.meshes[node.mesh];
				auto nodeModel = CreateModel(model, mesh);
				sceneNode->model = nodeModel;
			}
			parentNode->children.push_back(sceneNode);
			for (auto child : node.children) {
				AnalysisModel(model, child, sceneNode);
			}
		}
		/// <summary>
		/// 获取当前mesh的世界坐标
		/// </summary>
		/// <param name="postions"></param>
		/// <returns></returns>
		std::vector<float> GltfFileHandle::GetWorldPositions(const std::vector<float>& positions, int nodeIndex) {
			//得到变换矩阵
			//遍历node节点，得到mesh的最终的遍历矩阵
			//将position变为世界坐标并返回
			Eigen::Matrix4d worldMatrix = _worldMatrices[nodeIndex];

			std::vector<float> worldPostions;
			for (size_t i = 0; i < positions.size(); i += 3)
			{
				Eigen::Vector4d p(positions[i + 0], positions[i + 1], positions[i + 2], 1.0);
				Eigen::Vector4d pw = worldMatrix * p;   // 应用世界矩阵
				worldPostions.push_back(static_cast<float>(pw.x()));
				worldPostions.push_back(static_cast<float>(pw.y()));
				worldPostions.push_back(static_cast<float>(pw.z()));
			}
			return worldPostions;
		}
		/// <summary>
		/// 获取全部节点的变换矩阵
		/// </summary>
		void GltfFileHandle::GetNodeMatrix(const tinygltf::Model& model) {
			_worldMatrices.resize(model.nodes.size());
			const tinygltf::Scene& scene = model.scenes[model.defaultScene];
			for (int rootNode : scene.nodes) {
				Eigen::Matrix4d identity = Eigen::Matrix4d::Identity();
				ComputeWorldMatrix(model, rootNode, identity, _worldMatrices);
			}
		}
		/// <summary>
		/// 计算变换矩阵
		/// </summary>
		/// <param name="model"></param>
		/// <param name="nodeIndex"></param>
		/// <param name="parentWorld"></param>
		/// <param name="worldMatrices"></param>
		void GltfFileHandle::ComputeWorldMatrix(const tinygltf::Model& model, int nodeIndex, const Eigen::Matrix4d& parentWorld, std::vector<Eigen::Matrix4d>& worldMatrices)
		{
			const tinygltf::Node& node = model.nodes[nodeIndex];
			Eigen::Matrix4d local = GetLocalMatrix(node);
			Eigen::Matrix4d world = parentWorld * local;
			worldMatrices[nodeIndex] = world;
			for (int child : node.children)
				ComputeWorldMatrix(model, child, world, worldMatrices);
		}
		/// <summary>
		/// 得到节点的变换矩阵
		/// </summary>
		/// <param name="node"></param>
		/// <returns></returns>
		Eigen::Matrix4d GltfFileHandle::GetLocalMatrix(const tinygltf::Node& node)
		{
			Eigen::Matrix4d matrix = Eigen::Matrix4d::Identity();

			// glTF 如果直接提供 matrix
			if (!node.matrix.empty() && node.matrix.size() == 16) {
				Eigen::Map<const Eigen::Matrix<double, 4, 4, Eigen::ColMajor>> m(node.matrix.data());
				matrix = m;
			}
			else {
				// 如果没有 matrix，就用 T * R * S
				Eigen::Vector3d t = Eigen::Vector3d::Zero();
				if (!node.translation.empty()) {
					t = Eigen::Vector3d(node.translation[0], node.translation[1], node.translation[2]);
				}

				Eigen::Quaterniond q = Eigen::Quaterniond::Identity();
				if (!node.rotation.empty()) {
					q = Eigen::Quaterniond(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);
				}

				Eigen::Vector3d s(1.0, 1.0, 1.0);
				if (!node.scale.empty()) {
					s = Eigen::Vector3d(node.scale[0], node.scale[1], node.scale[2]);
				}

				Eigen::Matrix4d tm = Eigen::Matrix4d::Identity();
				tm.block<3, 1>(0, 3) = t;

				Eigen::Matrix4d rm = Eigen::Matrix4d::Identity();
				rm.block<3, 3>(0, 0) = q.toRotationMatrix();

				Eigen::Matrix4d sm = Eigen::Matrix4d::Identity();
				sm(0, 0) = s.x();
				sm(1, 1) = s.y();
				sm(2, 2) = s.z();

				matrix = tm * rm * sm;
			}
			return matrix;
		}

		/// <summary>
		/// 获取MeshData的数据
		/// </summary>
		/// <returns></returns>
		//std::vector<int, std::shared_ptr<Graphics::Mesh>> GltfFileHandle::GetMeshDatas() {
		//	return _meshDatas;
		//}

		tinygltf::Model GltfFileHandle::GetModel() {
			return _model;
		}
		/*std::unordered_map<int, int, std::shared_ptr<Graphics::Mesh>> GltfFileHandle::GetNodeMeshDatas() {
			return _nodeMeshDataMap;
		}*/
		std::shared_ptr<Robot::Robot> GltfFileHandle::CreateRobotFromGltfNode(const tinygltf::Model& model, int nodeIndex)
		{
			auto node = model.nodes[nodeIndex];
			std::shared_ptr<Robot::Robot> robot = std::make_shared<Robot::Robot>();
			return robot;
		}
		void GltfFileHandle::CreateRobot(const tinygltf::Model& model, int nodeIndex, const std::shared_ptr<Robot::Robot>& robot) {
			auto node = model.nodes[nodeIndex];
			if (node.name.find("Baseframe") != std::string::npos ||
				node.name.find("J1") != std::string::npos ||
				node.name.find("J2") != std::string::npos ||
				node.name.find("J3") != std::string::npos ||
				node.name.find("J4") != std::string::npos ||
				node.name.find("J5") != std::string::npos
				)
			{
				std::shared_ptr<Robot::Joint> joint = std::make_shared<Robot::Joint>();
				robot->joints.push_back(joint);

				std::shared_ptr<Robot::Link> link = std::make_shared<Robot::Link>();
				CreateLink(model, node.children[1], link);
				robot->links.push_back(link);

				CreateRobot(model, node.children[0], robot);
			}
		}
		void GltfFileHandle::CreateLink(const tinygltf::Model& model, int nodeIndex, const std::shared_ptr<Robot::Link>& link) {
			auto node = model.nodes[nodeIndex];
			if (node.mesh > 0) {
				auto nodeModel = CreateModel(model, model.meshes[node.mesh]);
				link->model = nodeModel;
				return;
			}
			for (int child : node.children)
			{
				CreateLink(model, child, link);
			}
		}
		std::shared_ptr<Graphics::Model> GltfFileHandle::CreateModel(const tinygltf::Model& model, const tinygltf::Mesh& mesh) {
			std::shared_ptr<Graphics::Model> nodeModel = std::make_shared<Graphics::Model>();
			for (const auto& prim : mesh.primitives) {
				std::shared_ptr<Graphics::Geometry> geometry = std::make_shared<Graphics::Geometry>();
				std::shared_ptr<Graphics::Mesh> modelMesh = std::make_shared<Graphics::Mesh>();
				//获取顶点位置
				std::vector<float> positions;
				auto accessorIndex = prim.attributes.find("POSITION");
				if (accessorIndex != prim.attributes.end()) {
					const tinygltf::Accessor& accessor = model.accessors[accessorIndex->second];
					const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = model.buffers[view.buffer];
					const unsigned char* dataPtr = buffer.data.data() + view.byteOffset + accessor.byteOffset;
					size_t count = accessor.count;
					size_t stride = accessor.ByteStride(view);
					if (stride == 0) {
						stride = tinygltf::GetNumComponentsInType(accessor.type) * tinygltf::GetComponentSizeInBytes(accessor.componentType);
					}
					for (size_t i = 0; i < count; ++i) {
						const float* src = reinterpret_cast<const float*>(dataPtr + i * stride);
						for (int j = 0; j < tinygltf::GetNumComponentsInType(accessor.type); ++j) {
							positions.push_back(src[j]);
						}
					}
				}

				//获取索引
				std::vector<unsigned int> indices;
				if (prim.indices >= 0) {
					const tinygltf::Accessor& accessor = model.accessors[prim.indices];
					const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = model.buffers[view.buffer];
					const unsigned char* dataPtr = buffer.data.data() + view.byteOffset + accessor.byteOffset;
					for (size_t k = 0; k < accessor.count; ++k) {
						uint32_t index = 0;
						switch (accessor.componentType) {
						case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
							index = *(reinterpret_cast<const uint8_t*>(dataPtr + k));
							break;
						case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
							index = *(reinterpret_cast<const uint16_t*>(dataPtr + k * sizeof(uint16_t)));
							break;
						case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
							index = *(reinterpret_cast<const uint32_t*>(dataPtr + k * sizeof(uint32_t)));
							break;
						}
						indices.push_back(index);
					}
				}
				else {
					//没有索引，去重，并重建索引
					std::unordered_map<QuantizedVertex, unsigned int, QuantizedHash, QuantizedEqual> vertexMap;
					std::vector<QuantizedVertex> uniquePnts;
					std::vector<float> tmpPositions;
					for (size_t i = 0; i + 2 < positions.size(); i += 3) {
						float x = positions[i];
						float y = positions[i + 1];
						float z = positions[i + 2];
						/*Graphics::Point p;
						p.x = x;
						p.y = y;
						p.z = z;*/
						QuantizedVertex qv = QuantizeVertex(x, y, z);
						auto it = vertexMap.find(qv);
						if (it != vertexMap.end())
						{
							// 已存在，直接复用索引
							indices.push_back(it->second);
						}
						else
						{
							uint32_t newIndex = static_cast<uint32_t>(uniquePnts.size());
							uniquePnts.push_back(qv);
							tmpPositions.push_back(x);
							tmpPositions.push_back(y);
							tmpPositions.push_back(z);
							vertexMap[qv] = newIndex;
							indices.push_back(newIndex);
						}
					}
					positions.clear();
					positions = tmpPositions;
					isRebuildIndex = true;
				}
				modelMesh->vertices.insert(modelMesh->vertices.end(), std::make_move_iterator(positions.begin()), std::make_move_iterator(positions.end()));

				//得到世界坐标
				//std::vector<float> worldPositions = GetWorldPositions(positions, nodeIndex);

				//获取法向量
				if (prim.mode == TINYGLTF_MODE_TRIANGLES)
				{
					std::vector<float> normals;
					auto normalAccessorIndex = prim.attributes.find("NORMAL");
					if (normalAccessorIndex != prim.attributes.end()) {
						const tinygltf::Accessor& accessor = model.accessors[normalAccessorIndex->second];
						const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
						const tinygltf::Buffer& buffer = model.buffers[view.buffer];
						const unsigned char* dataPtr = buffer.data.data() + view.byteOffset + accessor.byteOffset;
						size_t count = accessor.count;
						size_t stride = accessor.ByteStride(view);
						if (stride == 0) {
							stride = tinygltf::GetNumComponentsInType(accessor.type) * tinygltf::GetComponentSizeInBytes(accessor.componentType);
						}
						for (size_t i = 0; i < count; ++i) {
							const float* src = reinterpret_cast<const float*>(dataPtr + i * stride);
							for (int j = 0; j < tinygltf::GetNumComponentsInType(accessor.type); ++j) {
								normals.push_back(src[j]);
							}
						}
					}
					else
					{
						normals.clear();
						normals.resize(positions.size(), 0.0f);  // 与 positions 同大小

						// 遍历每个三角面
						for (size_t i = 0; i < indices.size(); i += 3)
						{
							uint32_t i0 = indices[i] * 3;
							uint32_t i1 = indices[i + 1] * 3;
							uint32_t i2 = indices[i + 2] * 3;

							// 顶点位置
							Eigen::Vector3f v0(positions[i0], positions[i0 + 1], positions[i0 + 2]);
							Eigen::Vector3f v1(positions[i1], positions[i1 + 1], positions[i1 + 2]);
							Eigen::Vector3f v2(positions[i2], positions[i2 + 1], positions[i2 + 2]);

							// 面法线
							Eigen::Vector3f e1 = v1 - v0;
							Eigen::Vector3f e2 = v2 - v0;
							Eigen::Vector3f faceNormal = e1.cross(e2);

							// 累加到顶点
							normals[i0] += faceNormal.x();
							normals[i0 + 1] += faceNormal.y();
							normals[i0 + 2] += faceNormal.z();

							normals[i1] += faceNormal.x();
							normals[i1 + 1] += faceNormal.y();
							normals[i1 + 2] += faceNormal.z();

							normals[i2] += faceNormal.x();
							normals[i2 + 1] += faceNormal.y();
							normals[i2 + 2] += faceNormal.z();
						}

						// 归一化
						for (size_t i = 0; i < normals.size(); i += 3)
						{
							Eigen::Vector3f n(normals[i], normals[i + 1], normals[i + 2]);
							if (n.norm() > 1e-6f)
								n.normalize();
							normals[i] = n.x();
							normals[i + 1] = n.y();
							normals[i + 2] = n.z();
						}
					}
					if (isRebuildIndex)
					{
						normals.clear();
						//std::vector<float> normals;
						normals.resize(positions.size(), 0.0f);  // 与 positions 同大小
						// 遍历每个三角面
						for (size_t i = 0; i < indices.size(); i += 3)
						{
							uint32_t i0 = indices[i] * 3;
							uint32_t i1 = indices[i + 1] * 3;
							uint32_t i2 = indices[i + 2] * 3;

							// 顶点位置
							Eigen::Vector3f v0(positions[i0], positions[i0 + 1], positions[i0 + 2]);
							Eigen::Vector3f v1(positions[i1], positions[i1 + 1], positions[i1 + 2]);
							Eigen::Vector3f v2(positions[i2], positions[i2 + 1], positions[i2 + 2]);

							// 面法线
							Eigen::Vector3f e1 = v1 - v0;
							Eigen::Vector3f e2 = v2 - v0;
							Eigen::Vector3f faceNormal = e1.cross(e2);

							// 累加到顶点
							normals[i0] += faceNormal.x();
							normals[i0 + 1] += faceNormal.y();
							normals[i0 + 2] += faceNormal.z();

							normals[i1] += faceNormal.x();
							normals[i1 + 1] += faceNormal.y();
							normals[i1 + 2] += faceNormal.z();

							normals[i2] += faceNormal.x();
							normals[i2 + 1] += faceNormal.y();
							normals[i2 + 2] += faceNormal.z();
						}

						// 归一化
						for (size_t i = 0; i < normals.size(); i += 3)
						{
							Eigen::Vector3f n(normals[i], normals[i + 1], normals[i + 2]);
							if (n.norm() > 1e-6f)
								n.normalize();
							normals[i] = n.x();
							normals[i + 1] = n.y();
							normals[i + 2] = n.z();
						}
					}
					modelMesh->normals.insert(modelMesh->normals.end(), std::make_move_iterator(normals.begin()), std::make_move_iterator(normals.end()));
				}

				//获取颜色
				std::vector<float> colors;
				auto colorAccessorIndex = prim.attributes.find("COLOR_0");
				if (colorAccessorIndex != prim.attributes.end())
				{
					const tinygltf::Accessor& accessor = model.accessors[colorAccessorIndex->second];
					const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = model.buffers[view.buffer];
					const unsigned char* dataPtr = buffer.data.data() + view.byteOffset + accessor.byteOffset;
					size_t count = accessor.count;
					size_t stride = accessor.ByteStride(view);
					if (stride == 0) {
						stride = tinygltf::GetNumComponentsInType(accessor.type) * tinygltf::GetComponentSizeInBytes(accessor.componentType);
					}
					for (size_t i = 0; i < count; ++i) {
						const float* src = reinterpret_cast<const float*>(dataPtr + i * stride);
						for (int j = 0; j < tinygltf::GetNumComponentsInType(accessor.type); ++j) {
							colors.push_back(src[j]);
						}
					}
				}
				//如果没有颜色，设置默认白色
				if (colors.empty())
				{
					for (size_t i = 0; i < positions.size() / 3; ++i)
					{
						colors.push_back(1.0f); // R
						colors.push_back(1.0f); // G
						colors.push_back(1.0f); // B
						colors.push_back(1.0f); // A
					}
				}

				//获取材质
				int materialIndex = prim.material; // 这里就是材质索引
				if (materialIndex >= 0)
				{
					std::shared_ptr<Graphics::Material> modelMaterial = std::make_shared<Graphics::Material>();
					const auto& material = model.materials[materialIndex];
					// 访问 baseColorFactor、metallicFactor、roughnessFactor
					const auto& pbr = material.pbrMetallicRoughness;
					Eigen::Vector4d baseColor;
					if (pbr.baseColorFactor.size() == 4)
					{
						baseColor = Eigen::Vector4d(
							pbr.baseColorFactor[0],
							pbr.baseColorFactor[1],
							pbr.baseColorFactor[2],
							pbr.baseColorFactor[3]
						);
					}
					modelMaterial->baseColorFactor = baseColor;
					geometry->material = modelMaterial;
				}

				modelMesh->indices.insert(modelMesh->indices.end(), std::make_move_iterator(indices.begin()), std::make_move_iterator(indices.end()));
				modelMesh->colors.insert(modelMesh->colors.end(), std::make_move_iterator(colors.begin()), std::make_move_iterator(colors.end()));
				geometry->mesh = modelMesh;
				nodeModel->geometries.push_back(geometry);
			}
			return nodeModel;
		}
	}
}