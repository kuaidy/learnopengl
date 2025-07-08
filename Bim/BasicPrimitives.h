#pragma once
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <Mesh.h>

using namespace Bim;

class BasicPrimitives {
public:
	BasicPrimitives(QOpenGLFunctions_4_5_Core* qOpenGLFunction);
	~BasicPrimitives();

	Mesh CreateSphereMesh(float radius, int latSegments = 24, int lonSegments = 24);
	void TranslateMesh(Mesh &mesh,const QVector3D &offset);
	void Ball();
	QVector3D offset;
private:
	QOpenGLFunctions_4_5_Core* m_QOpengGlFunction;
	QOpenGLShaderProgram* m_BasicPrimitivesShader;
};