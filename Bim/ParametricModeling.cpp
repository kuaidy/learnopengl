#include "ParametricModeling.h"


ParametricModeling::ParametricModeling(QOpenGLFunctions_4_5_Core* qOpenGLFunction) {
	m_QOpenGLFunction = qOpenGLFunction;
	m_LineShader = new QOpenGLShaderProgram();
	if (!m_LineShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/LineVertexShader.glsl")) {
		qDebug() << "Vertex shader error:" << m_LineShader->log();
	}
	if (!m_LineShader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/LineFragmentShader.glsl")) {
		qDebug() << "Fragment shader error:" << m_LineShader->log();
	}
	if (!m_LineShader->link()) {
		qDebug() << "Shader program link error:" << m_LineShader->log();
	}
}

ParametricModeling::~ParametricModeling() {
	delete m_LineShader;
}

/// <summary>
///	绘制贝塞尔曲线
/// </summary>
/// <param name="controlPoints">控制点</param>
/// <param name="count">采样点</param>
void ParametricModeling::BezierCurves(std::vector<QVector3D> controlPoints, int count) {
	if (controlPoints.size() < 2) return;
	std::vector<QVector3D> samplingPoints;
	for (int i = 0; i < count; ++i) {
		//获取到采样点
		QVector3D samplingPoint = Decasteljau(controlPoints, i / count);
		samplingPoints.push_back(samplingPoint);
	}
	//绘制

}
/// <summary>
/// 排列组合
/// </summary>
/// <param name="n"></param>
/// <param name="k"></param>
/// <returns></returns>
unsigned long long ParametricModeling::Binomial(int n, int k) {
	unsigned long long res = 1;
	for (int i = 1; i <= k; ++i) {
		res = res * (n - i + 1) / i;
	}
	return res;
}

/// <summary>
/// n阶贝塞尔函数
/// </summary>
/// <param name="t"></param>
/// <param name="controlPoints"></param>
/// <returns></returns>
QVector3D ParametricModeling::BezierN(float t, const std::vector<QVector3D>& controlPoints) {
	QVector3D result{ 0.0f,0.0f,0.0f };
	int n = static_cast<int>(controlPoints.size());
	for (int i = 0; i <= n; ++i) {
		float coeff = Binomial(n, i) * std::pow(1 - t, n - i) * std::pow(t, i);
		result = result + controlPoints[i] * coeff;
	}
	return result;
}

/// <summary>
/// 递归插值
/// </summary>
/// <param name="controlPoints"></param>
/// <param name="t"></param>
/// <returns></returns>
QVector3D ParametricModeling::Decasteljau(const std::vector<QVector3D>& controlPoints, float t) {
	if (controlPoints.size() == 1) {
		return controlPoints[0];
	}
	std::vector<QVector3D> nextLevel;
	for (int i = 0; i < controlPoints.size() - 1; ++i) {
		QVector3D point = controlPoints[i] * (1 - t) + controlPoints[i + 1] * t;
		nextLevel.push_back(point);
	}
	return Decasteljau(nextLevel, t);
}