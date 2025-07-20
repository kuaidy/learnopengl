#pragma once
#include <string>
#include <glm/glm.hpp>

struct Instance {
	std::string guid;
	glm::mat4 transform;
	int instance_id;
	int element_id;
	bool is_selected = false;
};