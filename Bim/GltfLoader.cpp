#include "GltfLoader.h"
#include <Element.h>

/// <summary>
/// 模型文件加载
/// </summary>
/// <param name="fileName"></param>
/// <returns></returns>
bool GltfLoader::Load(const std::string fileName) {
	Model model;
	TinyGLTF loader;
	std::string err;
	std::string warn;
	bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, fileName);
	if (!warn.empty()) {
		printf("Warn: %s\n", warn.c_str());
	}
	if (!err.empty()) {
		printf("Err: %s\n", err.c_str());
	}
	if (!ret) {
		printf("Failed to parse glTF\n");
		return false;
	}
	//得到文件夹
	std::filesystem::path path(fileName);
	std::filesystem::path folderPath = path.parent_path();
	//获取模型数据
	for (int i = 0; i < model.nodes.size(); ++i) {
		Element element;
		int meshIndex = model.nodes[i].mesh;
		if (meshIndex < 0) continue;
		const tinygltf::Mesh& mesh = model.meshes[meshIndex];
		for (int j = 0; j < mesh.primitives.size(); ++j) {
			const tinygltf::Primitive& primitive = mesh.primitives[j];
			//获取顶点数据
			std::vector<glm::vec3> positions;
			auto itPosition = primitive.attributes.find("POSITION");
			if (itPosition != primitive.attributes.end()) {
				const tinygltf::Accessor& accessor = model.accessors[itPosition->second];
				std::vector<float> datas = GetAttributeData(model, accessor);
				for (size_t k = 0; k < datas.size(); k += 3) {
					positions.emplace_back(datas[k], datas[k + 1], datas[k + 2]);
				}
			}
			//法线数据
			std::vector<glm::vec3> normals;
			auto itNormal = primitive.attributes.find("NORMAL");
			if (itNormal != primitive.attributes.end()) {
				const tinygltf::Accessor& accessor = model.accessors[itNormal->second];
				std::vector<float> datas = GetAttributeData(model, accessor);
				for (size_t k = 0; k < datas.size(); k += 3) {
					normals.emplace_back(datas[k], datas[k + 1], datas[k + 2]);
				}
			}
			//TEXCOORD_0 纹理坐标
			std::vector<glm::vec2> uvs;
			auto itTexcoord = primitive.attributes.find("TEXCOORD_0");
			if (itTexcoord != primitive.attributes.end()) {
				const tinygltf::Accessor& accessor = model.accessors[itTexcoord->second];
				std::vector<float> datas = GetAttributeData(model, accessor);
				for (size_t k = 0; k < datas.size(); k += 2) {
					uvs.emplace_back(datas[k], datas[k + 1]);
				}
			}
			//构造顶点数据
			for (int k = 0; k < positions.size(); ++k) {
				Bim::Vertex vertex;
				vertex.Position = QVector3D(positions[k].x, positions[k].y, positions[k].z);
				if (!normals.empty()) {
					vertex.Normal = QVector3D(normals[k].x, normals[k].y, normals[k].z);
				}
				if (!uvs.empty()) {
					vertex.TexCoords = QVector2D(uvs[k].x, uvs[k].y);
				}
				element.mesh.vertices.push_back(vertex);
			}
			//顶点索引
			if (primitive.indices >= 0) {
				const tinygltf::Accessor& accessor = model.accessors[primitive.indices];
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
					element.mesh.indices.push_back(index);
				}
			}
			//材质
			int materialIndex = primitive.material;
			if (materialIndex >= 0 && materialIndex < model.materials.size()) {
				const tinygltf::Material& material = model.materials[materialIndex];
				// 现在你可以访问 material 的各种属性
				if (material.pbrMetallicRoughness.baseColorTexture.index >= 0) {
					int textureIndex = material.pbrMetallicRoughness.baseColorTexture.index;
					const tinygltf::Texture& texture = model.textures[textureIndex];
					int imageIndex = texture.source;
					const tinygltf::Image& image = model.images[imageIndex];
					Bim::Texture bimTexture;
					bimTexture.type = "texture_diffuse";
					bimTexture.path = folderPath.string() + "/" + image.uri;
					element.mesh.textures.push_back(bimTexture);
				}
				else if (material.normalTexture.index >= 0) {
					int textureIndex = material.normalTexture.index;
					const tinygltf::Texture& texture = model.textures[textureIndex];
					int imageIndex = texture.source;
					const tinygltf::Image& image = model.images[imageIndex];
					Bim::Texture bimTexture;
					bimTexture.type = "texture_normal";
					bimTexture.path = folderPath.string() + "/" + image.uri;
					element.mesh.textures.push_back(bimTexture);
				}
			}
		}
		elements.push_back(element);

		//获取属性
		//名称
		ElementProperty propertyName;
		propertyName.node_index = i;
		propertyName.name = "名称";
		propertyName.value = model.nodes[i].name;
		element_properties.push_back(propertyName);

		//位置
		if (model.nodes[i].translation.size() == 3) {
			ElementProperty propertyPostion;
			propertyPostion.node_index = i;
			propertyPostion.name = "位置";
			propertyPostion.value = "";
			element_properties.push_back(propertyPostion);

			ElementProperty propertyPostionX;
			propertyPostionX.node_index = i;
			propertyPostionX.name = "x";
			propertyPostionX.value = std::to_string(model.nodes[i].translation[0]);
			propertyPostion.children.push_back(propertyPostionX);

			ElementProperty propertyPostionY;
			propertyPostionY.node_index = i;
			propertyPostionY.name = "y";
			propertyPostionY.value = std::to_string(model.nodes[i].translation[1]);
			propertyPostion.children.push_back(propertyPostionY);

			ElementProperty propertyPostionZ;
			propertyPostionZ.node_index = i;
			propertyPostionZ.name = "z";
			propertyPostionZ.value = std::to_string(model.nodes[i].translation[2]);
			propertyPostion.children.push_back(propertyPostionZ);
		}

	}
	//获取场景数据
	const tinygltf::Scene& scene = model.scenes[
		model.defaultScene >= 0 ? model.defaultScene : 0
	];
	for (int rootNode:scene.nodes) {
		CreateSceneTree(model, rootNode, scene_tree);
	}
	return true;
}
/// <summary>
/// 通用方法，获取数据
/// </summary>
/// <param name="model"></param>
/// <param name="accessor"></param>
/// <returns></returns>
std::vector<float> GltfLoader::GetAttributeData(const tinygltf::Model& model, const tinygltf::Accessor& accessor) {
	const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
	const tinygltf::Buffer& buffer = model.buffers[view.buffer];
	const unsigned char* dataPtr = buffer.data.data() + view.byteOffset + accessor.byteOffset;

	size_t count = accessor.count;
	size_t stride = accessor.ByteStride(view);
	if (stride == 0) {
		stride = tinygltf::GetNumComponentsInType(accessor.type) * tinygltf::GetComponentSizeInBytes(accessor.componentType);
	}
	std::vector<float> result;
	for (size_t i = 0; i < count; ++i) {
		const float* src = reinterpret_cast<const float*>(dataPtr + i * stride);
		for (int j = 0; j < tinygltf::GetNumComponentsInType(accessor.type); ++j) {
			result.push_back(src[j]);
		}
	}
	return result;
}
/// <summary>
/// 创建场景树
/// </summary>
/// <param name="model"></param>
/// <param name="nodeIndex"></param>
void GltfLoader::CreateSceneTree(const tinygltf::Model& gltfModel, int nodeIndex, SceneTree& parentSceneTree) {
	const auto& node = gltfModel.nodes[nodeIndex];
	SceneTree sceneTree;
	sceneTree.name = node.name;
	sceneTree.nodeIndex = nodeIndex;
	parentSceneTree.children.push_back(sceneTree);
	// 取出刚刚 push_back 的那一项的引用
	SceneTree& current = parentSceneTree.children.back();
	for (int childIndex : node.children) {
		CreateSceneTree(gltfModel, childIndex, current);
	}
}