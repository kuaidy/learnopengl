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
	/// <summary>
	/// 通过控制点绘制3阶样条曲线
	/// </summary>
	/// <param name="controlPoints"></param>
	void BSpline(const std::vector<QVector3D>& controlPoints);
	/// <summary>
	/// deboor 递推算法
	/// </summary>
	/// <param name="controlPoints">控制点</param>
	/// <param name="knots">节点向量</param>
	/// <param name="degree">阶</param>
	/// <param name="t"></param>
	QVector3D DeBoor(const std::vector<QVector3D> &controlPoints,const std::vector<float> &knots,int degree,float t);
	/// <summary>
	/// 生成均匀节点向量
	/// </summary>
	/// <param name="numControlPoints"></param>
	/// <param name="degree"></param>
	/// <returns></returns>
	std::vector<float> GenerateUniformKnots(int numControlPoints, int degree);
	std::vector<QVector3D> Vertices;
	QVector3D LastVertex;
private:
	QOpenGLFunctions_4_5_Core* m_QOpengGlFunction;
	QOpenGLShaderProgram* m_ModelLineShader;
};