#pragma once
#ifndef GLTFLOADER_H
#define GLTFLOADER_H

#include "tiny_gltf.h"
#include "FileLoader.h"
#include <filesystem>

using namespace tinygltf;

class GltfLoader :public FileLoader {
public:
	std::vector<Element> Load(const std::string fileName) override;
private:
	std::vector<float> GetAttributeData(const tinygltf::Model& model,const tinygltf::Accessor& accessor);
};

#endif // !GLTFLOADER_H

