#include "RectanglePlane.h"
RectanglePlane::RectanglePlane(std::shared_ptr<CommandManager> commandmanager, std::shared_ptr<Bim::Document> document)
	:m_command_manager(commandmanager),m_document(document)
{

}
RectanglePlane::~RectanglePlane() {

}
void RectanglePlane::Draw() {
	if (!m_initialized) {
		Initialize();
	}
	if (this->mesh.vertices.size() == 0) return;
	//加上最后一个点
	vector<Bim::Vertex> tmpVertexes = this->mesh.vertices;
	tmpVertexes.push_back(last_vertex);

	//默认工作平面为xy平面
	vector<Bim::Vertex> drawvertexes;
	std::vector<QVector3D> points = GeneratePositions(tmpVertexes[0].Position, tmpVertexes[1].Position,QVector3D(0,0,1));
	for (int i = 0; i < points.size(); ++i) {
		Bim::Vertex tmpvertex;
		tmpvertex.Position = points[i];
		drawvertexes.push_back(tmpvertex);
	}

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
	shader->bind();
	shader->setUniformValue("model", RenderContext::Instance().matrix_model);
	shader->setUniformValue("view", RenderContext::Instance().matrix_view);
	shader->setUniformValue("projection", RenderContext::Instance().matrix_projection); 

	gl->glBindVertexArray(vao);
	gl->glBindBuffer(GL_ARRAY_BUFFER, vbo);
	gl->glBufferData(GL_ARRAY_BUFFER, drawvertexes.size() * sizeof(Bim::Vertex), drawvertexes.data(), GL_STATIC_DRAW);
	gl->glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)drawvertexes.size());
	gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
	gl->glBindVertexArray(0);
	shader->release();
	if (this->mesh.vertices.size() == 2)
		m_command_manager->HandleFinish(m_document);
}
void RectanglePlane::Initialize() {

	shader = std::make_shared<QOpenGLShaderProgram>();
	if (!shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/Plane.vert")) {
		qDebug() << "Line Vertex shader error:" << shader->log();
		return;
	}
	if (!shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/Plane.frag")) {
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
	gl->glGenVertexArrays(1, &vao);
	gl->glBindVertexArray(vao);

	qDebug() << "Line vao :" << vao;

	gl->glGenBuffers(1, &vbo);
	gl->glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// 设置顶点属性
	gl->glEnableVertexAttribArray(0);
	gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Bim::Vertex), (void*)offsetof(Bim::Vertex, Position));
	// 解绑
	gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
	gl->glBindVertexArray(0);

	m_initialized = true;
}
/// <summary>
/// 生成工作平面上的矩形顶点
/// </summary>
/// <param name="firstpoint"></param>
/// <param name="secondpoint"></param>
/// <param name="normal"></param>
/// <returns></returns>
std::vector<QVector3D> RectanglePlane::GeneratePositions(QVector3D firstpoint, QVector3D secondpoint, QVector3D normal) {
	QVector3D n = normal.normalized();
	QVector3D worldlup = QVector3D(0,1,0);
	if (abs(QVector3D::dotProduct(n,worldlup))>0.99f) {
		worldlup = QVector3D(1,0,0);
	}
	QVector3D u = QVector3D::crossProduct(worldlup, n).normalized();
	QVector3D v = QVector3D::crossProduct(n, u).normalized();

	QVector3D delta = secondpoint - firstpoint;
	float ulen = QVector3D::dotProduct(delta,u);
	float vlen = QVector3D::dotProduct(delta,v);

	QVector3D a = firstpoint;
	QVector3D b = firstpoint + ulen * u;
	QVector3D c = firstpoint + ulen * u + vlen * v;
	QVector3D d = firstpoint + vlen * v;

	return { a,b,c,d };
}