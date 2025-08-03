#pragma once
#include <QOpenGLFunctions_4_5_Core>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLVersionFunctionsFactory>

class RenderContext
{
public:
	QOpenGLFunctions_4_5_Core* gl = nullptr;
	int width;
	int height;
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
	/// <summary>
	/// 相机位置
	/// </summary>
	QVector3D m_CameraPos;
	/// <summary>
	/// 相机目标位置
	/// </summary>
	QVector3D m_CameraTarget;
	/// <summary>
	/// 相机的上方向
	/// </summary>
	QVector3D m_Up;

	static RenderContext& Instance();
	void Initialize(QOpenGLFunctions_4_5_Core* glFunc, int width, int height, QMatrix4x4 matrixModel, QMatrix4x4 matrixView, QMatrix4x4 matrixProjection);
private:
	RenderContext() = default;
	~RenderContext() = default;

	RenderContext(const RenderContext&) = delete;
	RenderContext& operator=(const RenderContext&) = delete;
};

