#include "BasicPrimitives.h"

BasicPrimitives::BasicPrimitives(QOpenGLFunctions_4_5_Core* qOpenGLFunction) {
	m_QOpengGlFunction = qOpenGLFunction;
	m_BasicPrimitivesShader = new QOpenGLShaderProgram();
	if (!m_BasicPrimitivesShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/BasicPrimitivesVertexShader.glsl")) {
		qDebug() << "Model BasicPrimitives Vertex shader error:" << m_BasicPrimitivesShader->log();
	}
	if (!m_BasicPrimitivesShader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/BasicPrimitivesFragmentShader.glsl")) {
		qDebug() << "Model BasicPrimitives Fragment shader error:" << m_BasicPrimitivesShader->log();
	}
	if (!m_BasicPrimitivesShader->link()) {
		qDebug() << "Model BasicPrimitives Shader program link error:" << m_BasicPrimitivesShader->log();
	}
}
BasicPrimitives::~BasicPrimitives() {

}

Mesh BasicPrimitives::CreateSphereMesh(float radius, int latSegments, int lonSegments) {
	Mesh mesh;
	for (int i = 0; i <= latSegments; ++i) {
		float theta = M_PI * i / latSegments;
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);
		for (int j = 0; j <= lonSegments; ++j) {
			float phi = M_PI * j / lonSegments;
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);

			QVector3D pos(radius * sinTheta * cosPhi, radius * cosTheta, radius * sinTheta * sinPhi);
			QVector3D normal = pos.normalized();
			Vertex vertex({ pos,normal, });
			mesh.vertices.push_back(vertex);
		}
	}
	for (int i = 0; i < latSegments; ++i) {
		for (int j = 0; j < lonSegments; ++j) {
			int first = i * (lonSegments + 1) + j;
			int second = first + lonSegments + 1;

			//每个格子拆分两个三角形
			mesh.indices.push_back(first);
			mesh.indices.push_back(second);
			mesh.indices.push_back(first + 1);

			mesh.indices.push_back(second);
			mesh.indices.push_back(second + 1);
			mesh.indices.push_back(first + 1);
		}
	}
	return mesh;
}

void BasicPrimitives::TranslateMesh(Mesh& mesh, const QVector3D& offset) {
	for (auto &v:mesh.vertices) {
		v.Position += offset;
	}
}

void BasicPrimitives::Ball() {
	Mesh mesh = CreateSphereMesh(1);
	TranslateMesh(mesh, offset);
	mesh.SetupMesh();
	mesh.Draw(*m_BasicPrimitivesShader);
}