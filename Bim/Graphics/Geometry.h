#pragma once
#pragma once
#include <vector>
#include "Mesh.h"
#include <memory>
#include "Material.h"

namespace Bim
{
	namespace Graphics {
		/// <summary>
		/// »æÖÆµ¥Ôª
		/// </summary>
		struct Geometry
		{
			std::shared_ptr<Mesh> mesh;
			std::shared_ptr<Material> material;
		};
	}
}