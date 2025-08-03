#include "Ball.h"
Ball::Ball(std::shared_ptr<CommandManager> ball_command_manager,
	std::shared_ptr<Bim::Document> ball_document) :
	m_command_manager(ball_command_manager),
	m_document(ball_document)
{
	for (int i = 0; i <= stacks; ++i) {
		float phi = M_PI * i / stacks;
		float y = radius * cos(phi);
		float r = radius * sin(phi);

		for (int j = 0; j <= slices; ++j) {
			float theta = 2.0f * M_PI * j / slices;
			float x = r * cos(theta);
			float z = r * sin(theta);

			QVector3D pos = QVector3D(x, y, z);
			QVector3D normal = QVector3D(x, y, z).normalized();
			Bim::Vertex vertex;
			vertex.Position = pos;
			vertex.Normal = normal;
			this->mesh.vertices.push_back(vertex);
		}
	}
	//生成三角形索引
	for (int i = 0; i <= stacks; ++i) {
		for (int j = 0; j < slices; ++j) {
			int first = i * (slices + 1) + j;
			int second = first + slices + 1;

			this->mesh.indices.push_back(first);
			this->mesh.indices.push_back(second);
			this->mesh.indices.push_back(first + 1);

			this->mesh.indices.push_back(second);
			this->mesh.indices.push_back(second + 1);
			this->mesh.indices.push_back(first + 1);
		}
	}
}
Ball::~Ball() {
}

void Ball::Initialize() {
	shader = std::make_shared<QOpenGLShaderProgram>();
	if (!shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/Ball.vert")) {
		qDebug() << "Ball Vertex shader error:" << shader->log();
		return;
	}
	if (!shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/Ball.frag")) {
		qDebug() << "Ball Fragment shader error:" << shader->log();
		return;
	}
	if (!shader->link()) {
		qDebug() << "Ball Shader program link error:" << shader->log();
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
	gl->glGenVertexArrays(1, &vao);
	gl->glBindVertexArray(vao);

	qDebug() << "Ball vao :" << vao;

	gl->glGenBuffers(1, &vbo);
	gl->glBindBuffer(GL_ARRAY_BUFFER, vbo);
	gl->glBufferData(GL_ARRAY_BUFFER, this->mesh.vertices.size() * sizeof(Bim::Vertex), this->mesh.vertices.data(), GL_STATIC_DRAW);

	gl->glGenBuffers(1, &ebo);
	gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mesh.indices.size() * sizeof(unsigned int), this->mesh.indices.data(), GL_STATIC_DRAW);

	// 设置顶点属性
	gl->glEnableVertexAttribArray(0);
	gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Bim::Vertex), (void*)offsetof(Bim::Vertex, Position));

	gl->glEnableVertexAttribArray(1);
	gl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Bim::Vertex), (void*)offsetof(Bim::Vertex, Normal));

	gl->glBindVertexArray(0);

	m_initialized = true;
}

void Ball::Draw() {
	if (!m_initialized) {
		Initialize();
	}
	shader->bind();
	shader->setUniformValue("model", RenderContext::Instance().matrix_model);
	shader->setUniformValue("view", RenderContext::Instance().matrix_view);
	shader->setUniformValue("projection", RenderContext::Instance().matrix_projection);
	auto* gl = RenderContext::Instance().gl;
	gl->glBindVertexArray(vao);
	gl->glDrawElements(GL_TRIANGLES, this->mesh.indices.size(), GL_UNSIGNED_INT, 0);
	gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
	gl->glBindVertexArray(0);
	shader->release();
}