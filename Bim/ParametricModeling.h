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
	/// ���Ʊ���������
	/// </summary>
	void BezierCurves(std::vector<QVector3D> controlPoints, int count);
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
private:
	QOpenGLFunctions_4_5_Core* m_QOpenGLFunction;
	QOpenGLShaderProgram* m_LineShader;
};