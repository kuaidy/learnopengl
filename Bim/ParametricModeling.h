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
	/// 绘制贝塞尔曲线
	/// </summary>
	void BezierCurves(std::vector<QVector3D> controlPoints, int count);
	/// <summary>
	/// 计算排列组合
	/// </summary>
	/// <param name="n"></param>
	/// <param name="k"></param>
	/// <returns></returns>
	unsigned long long Binomial(int n, int k);
	/// <summary>
	/// n阶贝塞尔函数
	/// </summary>
	/// <param name="t"></param>
	/// <param name="controlPoints"></param>
	/// <returns></returns>
	QVector3D BezierN(float t, const std::vector<QVector3D>& controlPoints);
	/// <summary>
	/// 递归插值
	/// </summary>
	/// <param name="controlPoints"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	QVector3D Decasteljau(const std::vector<QVector3D>& controlPoints, float t);
private:
	QOpenGLFunctions_4_5_Core* m_QOpenGLFunction;
	QOpenGLShaderProgram* m_LineShader;
};