#include "ModelLine.h"
ModelLine::ModelLine(QOpenGLFunctions_4_5_Core* qOpenGLFunction) {
	m_QOpengGlFunction = qOpenGLFunction;
	m_ModelLineShader = new QOpenGLShaderProgram();
	if (!m_ModelLineShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/ModelLineVertexShader.glsl")) {
		qDebug() << "Model Line Vertex shader error:" << m_ModelLineShader->log();
	}
	if (!m_ModelLineShader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/ModelLineFragmentShader.glsl")) {
		qDebug() << "Model Line Fragment shader error:" << m_ModelLineShader->log();
	}
	if (!m_ModelLineShader->link()) {
		qDebug() << "Model Line Shader program link error:" << m_ModelLineShader->log();
	}
}
ModelLine::	~ModelLine() {
	delete m_ModelLineShader;
}

void ModelLine::Draw() {

}