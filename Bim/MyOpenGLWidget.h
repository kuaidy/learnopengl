#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <assimp/scene.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <string>
#include <QOpenGLVersionFunctionsFactory>
#include <QMouseEvent>
#include "Model.h"
#include "Mesh.h"
#include "Mark.h"
#include "ParametricModeling.h"
#include <ModelLine.h>
#include <BasicPrimitives.h>
#include <Element.h>
#include "FileLoader.h"
#include <QPainter>
#include <Commands/CommandManager.h>
#include <Commands/CommandState.h>
#include "Document.h"

class MyOpenGLWidget :public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
	explicit MyOpenGLWidget(QWidget* parent = nullptr, 
		std::shared_ptr<CommandManager> commandManager = nullptr,
		std::shared_ptr<Document> document=nullptr);
	~MyOpenGLWidget();
	std::shared_ptr<FileLoader> file_loader;
	void CreateModelLine();
	void InitMesh();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
private:

	QOpenGLFunctions_4_5_Core* m_QOpengGlFunction;
	std::shared_ptr<CommandManager> m_CommandManager;
	std::shared_ptr<Document> m_Document;

	std::shared_ptr<ModelLine> m_ModelLine;
	std::shared_ptr<BasicPrimitives>m_BasicPrimitives;

	//Model* m_Model;
	QOpenGLShaderProgram* m_Shader;
	QOpenGLShaderProgram* m_PickShader;
	QOpenGLShaderProgram* m_TestShader;
	unsigned int VAO;
	unsigned int VBO;

	QVector3D m_CameraPos;
	QVector3D m_CameraTarget;
	QVector3D m_Up;

	vector<QVector3D> m_Points;
	/// <summary>
	/// 模型矩阵
	/// </summary>
	QMatrix4x4 m_MatrixModel;
	/// <summary>
	/// 观察矩阵
	/// </summary>
	QMatrix4x4 m_MatrixView;
	/// <summary>
	/// 投影矩阵
	/// </summary>
	QMatrix4x4 m_MatrixProjection;

	//QVector3D ScreenToWorld(int x, int y);
	/// <summary>
	/// 判断鼠标点击是否与模型的包围盒相交
	/// </summary>
	/// <returns></returns>
	bool RayIntersectsAABB(const QVector3D& rayOrigin,
		const QVector3D& rayDir,
		const QVector3D& boxMin,
		const QVector3D& boxMax,
		float& tmin,
		float& tmax);
	QVector3D ScreenPosToRayDir(int x, int y);
	/// <summary>
	/// 标注
	/// </summary>
	Mark* m_Mark;
	/// <summary>
	/// 参数化建模
	/// </summary>
	ParametricModeling* m_ParametricModeling;
	/// <summary>
	/// 缩放
	/// </summary>
	void Scale();
	bool m_IsAltPress = false;
	bool m_IsRightMousePress = false;
	QPoint m_RightMousePoint;
	float m_Distance = 3.0f;
	float m_Yaw = 0.0f;
	float m_Pitch = 0.0f;
	bool mesh_inited = false;
	int selected_object_id = -1;

	/// <summary>
	/// 模型加载时，相机适应
	/// </summary>
	void ResetCameraToFitModel(const std::vector<Element>& elements);

	unsigned int m_TriangleSSBO = 0;
	unsigned int m_RaySSBO = 0;
	unsigned int m_ResultSSBO = 0;
	unsigned int m_DebugSSBO = 0;

	//设置一个剖切面
	QVector4D m_ClipPlane = { 0.0f, 1.0f, 0.0f, -0.5f };

	void UploadTriangleData();

	void PerformPicking(const QPoint& mousePos);

	void Test();

	void MarkText();

	void MarkAngle();

	void MarkArea();
};

