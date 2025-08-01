#pragma once
#include <QOpenGLFunctions_4_5_Core>
#include <QMatrix4x4>

class RenderContext
{
public:
	QOpenGLFunctions_4_5_Core* gl = nullptr;
	int width;
	int height;
	/// <summary>
	/// 模型矩阵
	/// </summary>
	QMatrix4x4 MatrixModel;
	/// <summary>
	/// 观察矩阵
	/// </summary>
	QMatrix4x4 MatrixView;
	/// <summary>
	/// 投影矩阵
	/// </summary>
	QMatrix4x4 MatrixProjection;

	static RenderContext& Instance();
	void Initialize(QOpenGLFunctions_4_5_Core* glFunc, int width, int height, QMatrix4x4 matrixModel, QMatrix4x4 matrixView, QMatrix4x4 matrixProjection);
private:
	RenderContext() = default;
	~RenderContext() = default;

	RenderContext(const RenderContext&) = delete;
	RenderContext& operator=(const RenderContext&) = delete;
};

