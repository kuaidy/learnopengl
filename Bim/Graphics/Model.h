#pragma once
#include <vector>
#include <string>
#include <memory>

#include "Mesh.h"
#include "../Graphics/Geometry.h"

namespace Bim
{
	namespace Graphics {
		struct Model
		{
			std::vector<std::shared_ptr<Graphics::Geometry>> geometries;
			std::string nodeGuid;
		};
	}
}