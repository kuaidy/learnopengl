#include "MyOpenGLWidget.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <Mark.h>
#include <Model.h>

MyOpenGLWidget::MyOpenGLWidget(QWidget* parent):QOpenGLWidget(parent),m_program(nullptr) {

}
MyOpenGLWidget::~MyOpenGLWidget()
{
	delete m_program;
	delete m_Model;
}

void MyOpenGLWidget::initializeGL() {
	//初始化
	this->initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);

	m_program = new QOpenGLShaderProgram(this);
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/VertexShader.glsl")) {
		qDebug() << "Vertex shader error:" << m_program->log();
	}
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/FragmentShader.glsl")) {
		qDebug() << "Fragment shader error:" << m_program->log();
	}
	if (!m_program->link()) {
		qDebug() << "Shader program link error:" << m_program->log();
	}
	m_program->link();
	m_program->bind();
	auto func = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_5_Core>(QOpenGLContext::currentContext());
	m_Model = new Model(func);
	m_Model->LoadFile("C:/MyProject/LearnOpenGL/x64/Debug/Resources/backpack/backpack.obj");
};
void MyOpenGLWidget::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);
};
void MyOpenGLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//模型矩阵
	QMatrix4x4 model;
	model.translate(QVector3D(0.0f, 0.0f, 0.0f));
	model.scale(QVector3D(0.5f, 0.5f, 0.5f));
	model.rotate(qRadiansToDegrees(45.0),QVector3D(1.0f, 0.0f, 0.0f));
	m_program->setUniformValue("model", model);

	//观察举证
	QVector3D cameraPos(0.0f, 0.0f, 3.0f);
	QVector3D cameraTarget(0.0f, 0.0f, 0.0f);
	QVector3D up(0.0f, 1.0f, 0.0f);
	QMatrix4x4 view;
	view.lookAt(cameraPos, cameraTarget, up);
	m_program->setUniformValue("view", view);

	//投影矩阵
	QMatrix4x4 projection;
	projection.perspective(qRadiansToDegrees(45.0), this->width() / this->height(), 0.1, 100.0);
	m_program->setUniformValue("projection", projection);

	m_program->bind();
	m_Model->Draw(*m_program);
	m_program->release();
};
