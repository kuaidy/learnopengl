#pragma once
#include <string>
#include <vector>

struct SceneTree {
	std::string guid;
	std::string name;
	int nodeIndex;
	std::vector<SceneTree> children;
};