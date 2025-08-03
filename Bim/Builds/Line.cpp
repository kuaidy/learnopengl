#include "Line.h"
Line::Line(std::shared_ptr<CommandManager> line_command_manager,std::shared_ptr<Bim::Document> line_document):
	m_command_manager(line_command_manager),
	m_document(line_document)
{
	
}
Line::~Line() {
}
void Line::Draw() {
	if (!m_initialized) {
		Initialize();
	}
	if (this->mesh.vertices.size() == 0) return;
	//加上最后一个点
	vector<Bim::Vertex> tmpVertexes = this->mesh.vertices;
	tmpVertexes.push_back(last_vertex);

	shader->bind();
	//m_ModelLineShader->setUniformValue("model", model);
	shader->setUniformValue("view", matrix_view);
	shader->setUniformValue("projection", matrix_projection);

	opengl_funcs->glBindVertexArray(vao);
	opengl_funcs->glBindBuffer(GL_ARRAY_BUFFER, vbo);
	opengl_funcs->glBufferData(GL_ARRAY_BUFFER, tmpVertexes.size() * sizeof(Bim::Vertex), tmpVertexes.data(), GL_STATIC_DRAW);
	opengl_funcs->glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)tmpVertexes.size());
	opengl_funcs->glBindBuffer(GL_ARRAY_BUFFER, 0);
	opengl_funcs->glBindVertexArray(0);
	shader->release();
	if (this->mesh.vertices.size() == 2)
		m_command_manager->HandleFinish(m_document);
}

void Line::Initialize() {
	shader = std::make_shared<QOpenGLShaderProgram>();
	if (!shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/Line.vert")) {
		qDebug() << "Line Vertex shader error:" << shader->log();
		return;
	}
	if (!shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/Line.frag")) {
		qDebug() << "Line Fragment shader error:" << shader->log();
		return;
	}
	if (!shader->link()) {
		qDebug() << "Line Shader program link error:" << shader->log();
		return;
	}
	//初始化缓存数据
	auto* gl = RenderContext::Instance().gl;
	if (!gl) {
		qDebug() << "No OpenGL functions available!";
		return;
	}
	// 确保 OpenGL 函数已初始化
	if (!gl->initializeOpenGLFunctions()) {
		qDebug() << "Failed to initialize OpenGL functions!";
		return;
	}
	if (gl) {
		gl->initializeOpenGLFunctions();
	}
	opengl_funcs->glGenVertexArrays(1, &vao);
	opengl_funcs->glBindVertexArray(vao);

	qDebug() << "Line vao :" << vao;

	opengl_funcs->glGenBuffers(1, &vbo);
	opengl_funcs->glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// 设置顶点属性
	opengl_funcs->glEnableVertexAttribArray(0);
	opengl_funcs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Bim::Vertex), (void*)offsetof(Bim::Vertex, Position));
	// 解绑
	opengl_funcs->glBindBuffer(GL_ARRAY_BUFFER, 0);
	opengl_funcs->glBindVertexArray(0);

	m_initialized = true;
}