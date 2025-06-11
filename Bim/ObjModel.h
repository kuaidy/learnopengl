#pragma once
#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

class ObjModel:public Model
{
public:
	bool LoadFile(string path) override;
	void Draw() override;
};

