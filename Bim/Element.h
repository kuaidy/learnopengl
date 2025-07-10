#pragma once
#include <string>
#include "Mesh.h"

class Element {
public:
	std::string guid;
	std::string name;
	std::string typeName;
	Bim::Mesh mesh;
};