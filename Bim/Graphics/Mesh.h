#pragma once
#include <vector>

namespace Bim
{
	namespace Graphics {
		struct Mesh
		{
			std::vector<float> vertices;
			std::vector<unsigned int> indices;
			std::vector<float> normals;
			std::vector<float> colors;
		};
	}
}