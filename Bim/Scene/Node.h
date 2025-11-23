#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Graphics/Model.h"
#include <Eigen/Dense>

namespace Bim
{
	namespace Scene {
		struct Node {
			std::string guid;
			std::string name;
			std::shared_ptr<std::shared_ptr<Node>> parent;
			std::vector<std::shared_ptr<Node>> children;
			std::shared_ptr<Graphics::Model> model;
			Eigen::Matrix4d localMatrix;
			Eigen::Matrix4d globalMatrix;
		};
	}
}
