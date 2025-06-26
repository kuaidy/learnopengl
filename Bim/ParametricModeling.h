#pragma once
#include <vector>
#include <QVector3D>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <cmath>

using namespace std;

class ParametricModeling {
public:
	ParametricModeling(QOpenGLFunctions_4_5_Core* qOpenGLFunction);
	~ParametricModeling();

	/// <summary>
	/// ���߿��Ƶ�
	/// </summary>
	std::vector<QVector3D> ControlPoints;

	/// <summary>
	/// ���Ʊ���������
	/// </summary>
	void BezierCurves(int count,
		QMatrix4x4 model,
		QMatrix4x4 view,
		QMatrix4x4 projection);
	/// <summary>
	/// �����������
	/// </summary>
	/// <param name="n"></param>
	/// <param name="k"></param>
	/// <returns></returns>
	unsigned long long Binomial(int n, int k);
	/// <summary>
	/// n�ױ���������
	/// </summary>
	/// <param name="t"></param>
	/// <param name="controlPoints"></param>
	/// <returns></returns>
	QVector3D BezierN(float t, const std::vector<QVector3D>& controlPoints);
	/// <summary>
	/// �ݹ��ֵ
	/// </summary>
	/// <param name="controlPoints"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	QVector3D Decasteljau(const std::vector<QVector3D>& controlPoints, float t);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="curvePoints"></param>
	/// <param name="radius"></param>
	/// <param name="segmentPointsCount"></param>
	/// <param name="outVertices"></param>
	/// <param name="outNormals"></param>
	/// <param name="outIndices"></param>
	void GenerateCable(const std::vector<QVector3D>& curvePoints,
		float radius,
		int segmentPointsCount,
		QMatrix4x4 model,
		QMatrix4x4 view,
		QMatrix4x4 projection);
private:
	QOpenGLFunctions_4_5_Core* m_QOpenGLFunction;
	QOpenGLShaderProgram* m_LineShader;
};