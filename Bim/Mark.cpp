#include "Mark.h"


Mark::Mark() {
	m_QOpenGLFunctions= QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_5_Core>(QOpenGLContext::currentContext());
	m_Shader = new QOpenGLShaderProgram();
	if (!m_Shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/MarkVertexShader.glsl")) {
		qDebug() << "Vertex shader error:" << m_Shader->log();
	}
	if (!m_Shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/MarkFragmentShader.glsl")) {
		qDebug() << "Fragment shader error:" << m_Shader->log();
	}
	if (!m_Shader->link()) {
		qDebug() << "Shader program link error:" << m_Shader->log();
	}
}
