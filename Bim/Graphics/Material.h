#pragma once
#include <vector>
#include <Eigen/Dense>

namespace Bim
{
	namespace Graphics {
		struct Material
		{
			Eigen::Vector4d baseColorFactor;
			float metallicFactor = 1.0f;
			float roughnessFactor = 1.0f;
			int baseColorTextureId = -1;
			int normalTextureId = -1;
			int occlusionTextureId = -1;
		};
	}
}