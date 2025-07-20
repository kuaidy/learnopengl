#pragma once
#include "FileLoader.h"
class ObjLoader:public FileLoader {
public:
	bool Load(const std::string fileName) override;
};