#include "Line.h"
Line::Line(std::shared_ptr<CommandManager> commandManager,std::shared_ptr<Bim::Document> document):
	m_CommandManager(commandManager),
	m_Document(document){
	m_Shader = new QOpenGLShaderProgram();
	if (!m_Shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/Line.vert")) {
		qDebug() << "Line Vertex shader error:" << m_Shader->log();
	}
	if (!m_Shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/Line.frag")) {
		qDebug() << "Line Fragment shader error:" << m_Shader->log();
	}
	if (!m_Shader->link()) {
		qDebug() << "Line Shader program link error:" << m_Shader->log();

		m_QOpenGLFunction->glGenVertexArrays(1, &vao);
		m_QOpenGLFunction->glBindVertexArray(vao);
		m_QOpenGLFunction->glGenBuffers(1, &vbo);
		m_QOpenGLFunction->glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//m_QOpenGLFunction->glBufferData(GL_ARRAY_BUFFER, dataSize, dataPtr, GL_DYNAMIC_DRAW);
		// 设置顶点属性
		m_QOpenGLFunction->glEnableVertexAttribArray(0);
		m_QOpenGLFunction->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Bim::Vertex), (void*)offsetof(Bim::Vertex, Position));
		// 解绑
		m_QOpenGLFunction->glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_QOpenGLFunction->glBindVertexArray(0);
	}
}
Line::~Line() {
	delete m_Shader;
	//m_QOpenGLFunction->glDeleteBuffers(1, &vbo);
	//m_QOpenGLFunction->glDeleteVertexArrays(1, &vao);
}
void Line::Draw() {
	if (this->mesh.vertices.size() == 0) return;
	//加上最后一个点
	vector<Bim::Vertex> tmpVertexes = this->mesh.vertices;
	tmpVertexes.push_back(last_vertex);

	m_Shader->bind();
	//m_ModelLineShader->setUniformValue("model", model);
	m_Shader->setUniformValue("view", m_MatrixView);
	m_Shader->setUniformValue("projection", m_MatrixProjection);

	m_QOpenGLFunction->glBindVertexArray(vao);

	m_QOpenGLFunction->glBindBuffer(GL_ARRAY_BUFFER, vbo);
	m_QOpenGLFunction->glBufferData(GL_ARRAY_BUFFER, tmpVertexes.size() * sizeof(Bim::Vertex), tmpVertexes.data(), GL_STATIC_DRAW);
	m_QOpenGLFunction->glEnableVertexAttribArray(0);
	m_QOpenGLFunction->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Bim::Vertex), (void*)offsetof(Bim::Vertex, Position));

	m_QOpenGLFunction->glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)tmpVertexes.size());
	m_Shader->release();
	if (this->mesh.vertices.size() == 2)
		m_CommandManager->HandleFinish(m_Document);
}