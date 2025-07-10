#pragma once
#ifndef GLTFLOADER_H
#define GLTFLOADER_H

#include "tiny_gltf.h"
#include "FileLoader.h"
#include "SceneTree.h"
#include <filesystem>

using namespace tinygltf;

class GltfLoader :public FileLoader {
public:
	bool Load(const std::string fileName) override;
private:
	std::vector<float> GetAttributeData(const tinygltf::Model& model,const tinygltf::Accessor& accessor);
	void CreateSceneTree(const tinygltf::Model& model,int nodeIndex, SceneTree& parentSceneTree);
};

#endif // !GLTFLOADER_H

