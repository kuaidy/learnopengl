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
	/// ͨ�����Ƶ����3����������
	/// </summary>
	/// <param name="controlPoints"></param>
	void BSpline(const std::vector<QVector3D>& controlPoints);
	/// <summary>
	/// deboor �����㷨
	/// </summary>
	/// <param name="controlPoints">���Ƶ�</param>
	/// <param name="knots">�ڵ�����</param>
	/// <param name="degree">��</param>
	/// <param name="t"></param>
	QVector3D DeBoor(const std::vector<QVector3D> &controlPoints,const std::vector<float> &knots,int degree,float t);
	/// <summary>
	/// ���ɾ��Ƚڵ�����
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