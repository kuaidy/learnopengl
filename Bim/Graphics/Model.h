#pragma once
#include <vector>
#include <string>
#include <memory>

#include "Mesh.h"

namespace Bim
{
	namespace Graphics {
		struct Model
		{
			std::string nodeGuid;
			std::vector<std::shared_ptr<Mesh>> meshes;
		};
	}
}