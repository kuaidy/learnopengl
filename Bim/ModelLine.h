#pragma once
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <vector>

using namespace std;

class ModelLine
{
public:
	ModelLine(QOpenGLFunctions_4_5_Core* qOpenGLFunction);
	~ModelLine();
	void Draw();
	std::vector<QVector3D> Vertices;
	QVector3D LastVertex;
private:
	QOpenGLFunctions_4_5_Core* m_QOpengGlFunction;
	QOpenGLShaderProgram* m_ModelLineShader;
};