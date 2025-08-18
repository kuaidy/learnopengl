#pragma once
#include <string>
#include "Mesh.h"
#include "ElementType.h"
/// <summary>
/// 模型
/// </summary>
class Element {
public:
	std::string name;
	ElementType element_type;
	Bim::Mesh mesh;
	bool is_selected;
	int objectId;

	QOpenGLFunctions_4_5_Core* opengl_funcs;
	std::shared_ptr<QOpenGLShaderProgram> shader;

	/// <summary>
	/// 模型矩阵
	/// </summary>
	QMatrix4x4 matrix_model;
	/// <summary>
	/// 观察矩阵
	/// </summary>
	QMatrix4x4 matrix_view;
	/// <summary>
	/// 投影矩阵
	/// </summary>
	QMatrix4x4 matrix_projection;
	int width;
	int height;
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
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