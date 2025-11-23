#pragma once

#include "../Graphics/Model.h"
#include <Eigen/Dense>
namespace Bim
{
	namespace Robot 
	{
		struct Link
		{
			std::string name;
			std::shared_ptr<Graphics::Model> model;
			Eigen::Matrix4d localMatrix;
			Eigen::Matrix4d globalMatrix;
		};
	}
}