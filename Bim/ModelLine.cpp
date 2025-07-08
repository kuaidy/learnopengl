#include "ModelLine.h"
ModelLine::ModelLine(QOpenGLFunctions_4_5_Core* qOpenGLFunction) {
	m_QOpengGlFunction = qOpenGLFunction;
	m_ModelLineShader = new QOpenGLShaderProgram();
	if (!m_ModelLineShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/LineVertexShader.glsl")) {
		qDebug() << "Model Line Vertex shader error:" << m_ModelLineShader->log();
	}
	if (!m_ModelLineShader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/LineFragmentShader.glsl")) {
		qDebug() << "Model Line Fragment shader error:" << m_ModelLineShader->log();
	}
	if (!m_ModelLineShader->link()) {
		qDebug() << "Model Line Shader program link error:" << m_ModelLineShader->log();
	}
}
ModelLine::	~ModelLine() {
	delete m_ModelLineShader;
}

/// <summary>
/// b样条曲线，de-door递推算法
/// </summary>
void ModelLine::BSpline(const std::vector<QVector3D>& controlPoints, 
	QMatrix4x4 model,
	QMatrix4x4 view,
	QMatrix4x4 projection) {
	//控制点数
	int numControlPoints = controlPoints.size();
	//阶数
	int degree = 3;
	//采样点
	int curveResolution = 100;
	//节点向量
	std::vector<float> knots = GenerateUniformKnots(numControlPoints, 3);
	//开始节点
	float start = knots[degree];
	//结束节点
	float end = knots[numControlPoints];
	//曲线采样点
	std::vector<QVector3D>curvePoints;
	for (int i = 0; i < curveResolution; i++) {
		float t = start + (end - start) * float(i) / float(curveResolution);
		QVector3D curvePoint = DeBoor(controlPoints, knots, degree, t);
		curvePoints.push_back(curvePoint);
	}
	unsigned int VAO, VBO;
	m_QOpengGlFunction->glGenVertexArrays(1, &VAO);
	m_QOpengGlFunction->glBindVertexArray(VAO);

	m_QOpengGlFunction->glGenBuffers(1, &VBO);
	m_QOpengGlFunction->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	m_QOpengGlFunction->glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(QVector3D), curvePoints.data(), GL_STATIC_DRAW);

	m_QOpengGlFunction->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)0);
	m_QOpengGlFunction->glEnableVertexAttribArray(0);

	m_ModelLineShader->bind();

	//模型矩阵
	m_ModelLineShader->setUniformValue("model", model);

	//观察矩阵
	m_ModelLineShader->setUniformValue("view", view);

	//投影矩阵
	m_ModelLineShader->setUniformValue("projection", projection);

	m_QOpengGlFunction->glBindVertexArray(VAO);
	m_QOpengGlFunction->glDrawArrays(GL_LINE_STRIP, 0, curvePoints.size());

	m_ModelLineShader->release();
}
/// <summary>
/// 
/// </summary>
/// <param name="controlPoints"></param>
/// <param name="knots"></param>
/// <param name="degree"></param>
/// <param name="t"></param>
QVector3D ModelLine::DeBoor(const std::vector<QVector3D>& controlPoints, const std::vector<float>& knots, int degree, float t)
{
	int n = controlPoints.size();
	//找到t所在的节点区间
	int k = degree;
	while (k < knots.size() - 1 && t >= knots[k + 1]) {
		k++;
	}
	k = max(degree, min(k, n - 1));
	std::vector<QVector3D> d(degree + 1);
	for (int j = 0; j <= degree; j++)
	{
		int idx = k - degree + j;
		if (idx >= 0 && idx < n) {
			d[j] = controlPoints[idx];
		}
		else {
			d[j] = QVector3D(0, 0, 0);
		}
	}
	//递归计算
	for (int r = 1; r < degree; r++) {
		for (int j = degree; j >= r; j--) {
			int knotIdx1 = k - degree + j;
			int knotIdx2 = k - degree + j + degree - r + 1;
			if (knotIdx1 >= 0 && knotIdx2 < knots.size() && knotIdx1 < knots.size() && knotIdx2 >= 0) {
				float denom = knots[knotIdx2] - knots[knotIdx1];
				if (abs(denom) > 1e-6) {
					float alpha = (t - knots[knotIdx1]) / denom;
					//线性插值
					d[j] = (1.0f - alpha) * d[j - 1] + alpha * d[j];
				}
			}
		}
	}
	return d[degree];
}

/// <summary>
/// 生成均匀节点向量
/// </summary>
/// <param name="numControlPoints"></param>
/// <param name="degree"></param>
/// <returns></returns>
std::vector<float> ModelLine::GenerateUniformKnots(int numControlPoints, int degree) {
	int m = numControlPoints + degree + 1;
	std::vector<float> knots(m);
	for (int i = 0; i < m; ++i) {
		if (i <= degree) {
			knots[i] = 0.0f;
		}
		else if (i >= numControlPoints) {
			knots[i] = 1.0f;
		}
		else {
			knots[i] = float(i - degree) / float(numControlPoints - degree);
		}
	}
	return knots;
}