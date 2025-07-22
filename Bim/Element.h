#pragma once
#include <string>
#include "Mesh.h"
/// <summary>
/// 模型
/// </summary>
class Element {
public:
	std::string guid;
	std::string name;
	ElementType type;
	Bim::Mesh mesh;
	bool is_selected;
	int objectId;
};
/// <summary>
/// 构件类型
/// </summary>
enum struct ElementType {
	Point,
	Line,
	Surface,
	Volume
};
/// <summary>
/// 三角面
/// </summary>
struct GPUTriangle {
	glm::vec4 v0;
	glm::vec4 v1;
	glm::vec4 v2;
	int objectId;
	/// <summary>
	/// 和glsl中进行字节对齐
	/// </summary>
	int padding1;
	int padding2;
	int padding3;
};
struct GPURay {
	glm::vec4 origin;
	glm::vec4 direction;
};
struct GPUResult {
	int objectId;
	float minT;
};