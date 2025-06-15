#include "MyOpenGLWidget.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <Mark.h>
#include <Model.h>

MyOpenGLWidget::MyOpenGLWidget(QWidget* parent) :QOpenGLWidget(parent), m_Shader(nullptr) {

}
MyOpenGLWidget::~MyOpenGLWidget()
{
	delete m_Shader;
	delete m_CubeShader;
	delete m_Model;
}

void MyOpenGLWidget::initializeGL() {
	//初始化
	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);
	// 启用混合,用来让包围盒透明
	glEnable(GL_BLEND);
	// 设置混合函数
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // 启用线框模式
	m_Shader = new QOpenGLShaderProgram(this);
	if (!m_Shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/VertexShader.glsl")) {
		qDebug() << "Vertex shader error:" << m_Shader->log();
	}
	if (!m_Shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/FragmentShader.glsl")) {
		qDebug() << "Fragment shader error:" << m_Shader->log();
	}
	if (!m_Shader->link()) {
		qDebug() << "Shader program link error:" << m_Shader->log();
	}


	m_CubeShader = new QOpenGLShaderProgram(this);
	if (!m_CubeShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/MarkVertexShader.glsl")) {
		qDebug() << "Vertex shader error:" << m_CubeShader->log();
	}
	if (!m_CubeShader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/MarkFragmentShader.glsl")) {
		qDebug() << "Fragment shader error:" << m_CubeShader->log();
	}
	if (!m_CubeShader->link()) {
		qDebug() << "Shader program link error:" << m_CubeShader->log();
	}


	auto func = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_5_Core>(QOpenGLContext::currentContext());
	m_Model = new Model(func);
	m_Model->LoadFile("C:/MyProject/LearnOpenGL/Resources/labixiaoxin/62b01271ee64be39728ffda6d1a6f53a.obj");
	//m_Model->LoadFile("C:/MyProject/LearnOpenGL/x64/Debug/Resources/backpack/backpack.obj");

	//m_program = new QOpenGLShaderProgram(this);
	//if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/MarkVertexShader.glsl")) {
	//	qDebug() << "Vertex shader error:" << m_program->log();
	//}
	//if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/MarkFragmentShader.glsl")) {
	//	qDebug() << "Fragment shader error:" << m_program->log();
	//}
	//if (!m_program->link()) {
	//	qDebug() << "Shader program link error:" << m_program->log();
	//}

};
void MyOpenGLWidget::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);
};
void MyOpenGLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_Shader->bind();
	//模型矩阵
	QMatrix4x4 model;
	model.translate(QVector3D(0.0f, 0.0f, 0.0f));
	model.scale(QVector3D(1.0f, 1.0f, 1.0f));
	//model.rotate(45.0, QVector3D(1.0f, 0.0f, 0.0f));
	model.rotate(45.0, QVector3D(0.0f, 1.0f, 0.0f));
	m_MatrixModel = model;
	m_Shader->setUniformValue("model", model);

	////观察举证
	QVector3D cameraPos(0.0f, 0.0f, 3.0f);
	QVector3D cameraTarget(0.0f, 0.0f, 0.0f);
	QVector3D up(0.0f, 1.0f, 0.0f);
	QMatrix4x4 view;
	view.lookAt(cameraPos, cameraTarget, up);
	m_MatrixView = view;
	m_Shader->setUniformValue("view", view);

	////投影矩阵
	QMatrix4x4 projection;
	projection.perspective(45.0, (float)this->width() / (float)this->height(), 0.1, 100.0);
	m_MatrixProjection = projection;
	m_Shader->setUniformValue("projection", projection);

	//是否选中，选中则高亮
	m_Shader->setUniformValue("isSelected", m_IsSelected);

	m_Model->Draw(*m_Shader);
	m_Shader->release();

	//m_Model->ShowBindingBox(this->width(), this->height(), *m_CubeShader, m_MatrixModel, m_MatrixView, m_MatrixProjection);
};

void MyOpenGLWidget::mousePressEvent(QMouseEvent* event) {
	QVector3D p = ScreenToWorld(event->pos().x(), event->pos().y());
	m_Points.push_back(p);

	//获取点击的射线
	QVector3D cameraPos(0.0f, 0.0f, 3.0f);
	QVector3D rayDir = ScreenPosToRayDir(event->pos().x(),event->pos().y());
	
	QMatrix4x4 invModel = m_MatrixModel.inverted();
	QVector3D localOrigin = invModel.map(cameraPos);
	QVector3D localDir = (invModel.map(cameraPos + rayDir) - localOrigin).normalized();
	float tMin, tMax;
	if (RayIntersectsAABB(localOrigin,localDir,m_Model->m_BindingBox.Min,m_Model->m_BindingBox.Max, tMin,tMax)) {
		m_IsSelected = true;
	}
	else {
		m_IsSelected = false;
	}
	update();
}


QVector3D MyOpenGLWidget::ScreenToWorld(int x, int y) {
	QVector4D ndc(((2.0f * x) / width() - 1.0f), 1.0f - (2.0f * y) / height(), 0.0f * 2.0f - 1.0f, 1.0f);
	QMatrix4x4 projection, view;
	projection.perspective(45.0f, float(this->width()) / height(), 0.1f, 100.0f);
	view.lookAt(QVector3D(0.0, 0.0, -3.0), QVector3D(0.0, 0.0, 0.0), { 0,1,0 });
	QMatrix4x4 inv = (projection * view).inverted();
	QVector4D worldPos = inv * ndc;
	return worldPos.toVector3DAffine();
	// 
	//int viewport[4];
	////opengl 视口，标准设备坐标，x,y,width,height
	//glGetIntegerv(GL_VIEWPORT, viewport);
	//float winx = x;
	//float winy = viewport[3] - y;//opengl 和 屏幕 y 是相反的
	//float winz;

	//glReadPixels(x, winy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winz);
	//if (winz == 1.0f) winz = 0.99f;

	//QMatrix4x4 projection, view;
	//projection.perspective(45.0f, float(this->width()) / height(), 0.1f, 100.0f);
	//view.lookAt(QVector3D(0.0, 0.0, -3.0), QVector3D(0.0, 0.0, 0.0), { 0,1,0 });

	//QMatrix4x4 inv = (projection * view).inverted();
	//QVector4D worldPos = inv * QVector4D((2.0f * winx) / width() - 1.0f,
	//	1.0f - (2.0f * winy) / height(),
	//	2.0f * winz - 1.0f, 1.0f);
	//worldPos /= worldPos.w();
	//return worldPos.toVector3D();
}

/// <summary>
/// 判断鼠标点击是否与模型的包围盒相交
/// </summary>
/// <returns></returns>
bool MyOpenGLWidget::RayIntersectsAABB(const QVector3D& rayOrigin,
	const QVector3D& rayDir,
	const QVector3D& boxMin,
	const QVector3D& boxMax,
	float& tmin,
	float& tmax) {
	tmin = -std::numeric_limits<float>::infinity();
	tmax = std::numeric_limits<float>::infinity();

	for (int i = 0; i < 3; ++i) {
		float invD = 1.0 / rayDir[i];
		float t0 = (boxMin[i] - rayOrigin[i]) * invD;
		float t1 = (boxMax[i] - rayOrigin[i]) * invD;
		if (invD < 0.0f) std::swap(t0, t1);
		tmin = std::max(tmin, t0);
		tmax = std::min(tmax, t1);
		if (tmax < tmin) {
			return false;
		}
	}
	return true;
}
/// <summary>
/// 射线
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
QVector3D MyOpenGLWidget::ScreenPosToRayDir(int x, int y) {
	float ndcX = (2.0f * x) / width() - 1.0f;
	float ndcY = 1.0f - (2.0f * y) / height();
	QVector4D rayClip(ndcX,ndcY,-1.0,1.0);

	QMatrix4x4 invProjView = (m_MatrixProjection * m_MatrixView).inverted();
	QVector4D rayWorld = invProjView * rayClip;
	rayWorld /= rayWorld.w();

	QVector3D cameraPos = m_MatrixView.inverted().column(3).toVector3D();
	QVector3D dir = (rayWorld.toVector3D() - cameraPos).normalized();
	return dir;
}