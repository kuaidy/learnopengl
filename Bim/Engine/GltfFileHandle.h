#ifndef GLTFHANDLE_H
#define GLTFHANDLE_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <Eigen/Dense>

#include "../Graphics/Mesh.h"
#include "../tiny_gltf.h"
#include "Context.h"
#include "../Robot/Robot.h"

namespace Bim 
{
	namespace Engine 
	{
		class GltfFileHandle {
		public:
			GltfFileHandle();
			bool ReadFile(const std::filesystem::path& filepath);
			bool ReadFile(const unsigned char* data, size_t size);
			bool WriteFile(const std::filesystem::path& filepath);
			std::vector<float> GetWorldPositions(const std::vector<float>& position, int nodeIndex);
			//std::vector<std::shared_ptr<Graphics::Mesh>> GetMeshDatas();
			void AnalysisModel(const tinygltf::Model& model, int nodeIndex,const std::shared_ptr<Scene::Node>& node);
			tinygltf::Model GetModel();
			void ComputeWorldMatrix(const tinygltf::Model& model, int nodeIndex, const Eigen::Matrix4d& parentWorld, std::vector<Eigen::Matrix4d>& worldMatrices);
			Eigen::Matrix4d GetLocalMatrix(const tinygltf::Node& node);
			void GetNodeMatrix(const tinygltf::Model& model);
			//std::unordered_map<int, std::shared_ptr<Graphics::Mesh>> GetNodeMeshDatas();
			std::shared_ptr<Robot::Robot> CreateRobotFromGltfNode(const tinygltf::Model& model, int nodeIndex);
		private:
			std::vector<std::shared_ptr<Graphics::Mesh>> _meshDatas;
			tinygltf::Model _model;
			std::vector<Eigen::Matrix4d> _worldMatrices;
			std::unordered_map<int, std::shared_ptr<Graphics::Mesh>> _nodeMeshDataMap;
		};
	}
}
#endif
