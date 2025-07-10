#pragma once
#include "FileLoader.h"
class ObjLoader:public FileLoader {
public:
	std::vector<Element> Load(const std::string fileName) override;
};