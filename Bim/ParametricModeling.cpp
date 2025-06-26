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
void ParametricModeling::BezierCurves(int count,
	QMatrix4x4 model,
	QMatrix4x4 view,
	QMatrix4x4 projection) {
	if (this->ControlPoints.size() < 2) return;
	std::vector<QVector3D> samplingPoints;
	for (int i = 0; i < count; ++i) {
		//获取到采样点
		QVector3D samplingPoint = Decasteljau(this->ControlPoints, (float)i / count);
		samplingPoints.push_back(samplingPoint);
	}
	//绘制
	unsigned int vao, vbo, ebo;
	m_QOpenGLFunction->glGenVertexArrays(1, &vao);
	m_QOpenGLFunction->glBindVertexArray(vao);

	m_QOpenGLFunction->glGenBuffers(1, &vbo);
	m_QOpenGLFunction->glBindBuffer(GL_ARRAY_BUFFER, vbo);
	m_QOpenGLFunction->glBufferData(GL_ARRAY_BUFFER, samplingPoints.size() * sizeof(QVector3D), samplingPoints.data(), GL_STATIC_DRAW);

	//m_QOpenGLFunction->glGenBuffers(1, &ebo);
	//m_QOpenGLFunction->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//m_QOpenGLFunction->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	m_QOpenGLFunction->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)0);
	m_QOpenGLFunction->glEnableVertexAttribArray(0);

	m_LineShader->bind();
	//模型矩阵
	//m_Shader->setUniformValue("model", model);
	//观察矩阵
	m_LineShader->setUniformValue("view", view);
	//投影矩阵
	m_LineShader->setUniformValue("projection", projection);
	//m_QOpenGLFunction->glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	m_QOpenGLFunction->glDrawArrays(GL_LINE_STRIP, 0, samplingPoints.size());
	m_LineShader->release();

	GenerateCable(samplingPoints, 0.1, 24, model, view, projection);
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

//通过贝塞尔曲线绘制电缆
void ParametricModeling::GenerateCable(const std::vector<QVector3D>& curvePoints,
	float radius,
	int segmentPointsCount,
	QMatrix4x4 model,
	QMatrix4x4 view,
	QMatrix4x4 projection) {
	int n = curvePoints.size();
	if (n < 2)return;

	//顶点坐标
	std::vector<QVector3D> outVertices;
	//法向量
	std::vector<QVector3D> outNormals;
	//索引
	std::vector<unsigned int> outIndices;

	const float twoPi = 2 * M_PI;

	//获取所有截面的点
	//for (int i = 0; i < n; ++i) {
	//	QVector3D p = curvePoints[i];
	//	//计算切线方向，曲线上前后两个点相减，近似
	//	QVector3D T;
	//	if (i == 0) {
	//		T = (curvePoints[i + 1] - p).normalized();
	//	}
	//	else if (i == n - 1) {
	//		T = (p - curvePoints[i - 1]).normalized();
	//	}
	//	else {
	//		T = (curvePoints[i + 1] - curvePoints[i - 1]).normalized();
	//	}
	//	//构造正交基，局部坐标tbn
	//	QVector3D N = QVector3D::crossProduct(T, QVector3D(0, 1, 0)).normalized();
	//	if (N.length() < 0.001f) {
	//		N = QVector3D::crossProduct(T, QVector3D(1, 0, 0)).normalized();
	//	}
	//	QVector3D B = QVector3D::crossProduct(T, N).normalized();
	//	//生成截面点
	//	for (int j = 0; j < segmentPointsCount; ++j) {
	//		float angle = j * twoPi / segmentPointsCount;
	//		QVector3D offset = N * cos(angle) * radius + B * sin(angle) * radius;
	//		QVector3D vertex = p + offset;
	//		outVertices.push_back(vertex);
	//		outNormals.push_back(offset.normalized());
	//	}
	//}

	//优化为平行传输法，修改突然的变向，导致法向量不连续的问题
	//获取所有截面的点
	QVector3D prevT, prevN;
	prevT = (curvePoints[1] - curvePoints[0]).normalized();
	QVector3D up = QVector3D(0, 1, 0);
	if (abs(QVector3D::dotProduct(prevT, up)) > 0.99f) {
		up = QVector3D(1, 0, 0); // fallback 避免 cross 得到0向量
	}
	prevN = QVector3D::crossProduct(prevT, up).normalized();

	for (int i = 1; i < n; ++i) {
		QVector3D N, B;
		//计算切线方向，曲线上前后两个点相减，近似
		QVector3D T = (curvePoints[i] - curvePoints[i - 1]).normalized();
		QVector3D axis = QVector3D::crossProduct(prevT, T).normalized();
		float angle = acos(QVector3D::dotProduct(prevT, T));
		if (axis.length() > 1e-6) {
			//平行传输法，旋转切线方向
			QMatrix4x4 rot;
			rot.rotate(angle, axis);
			N = rot.mapVector(prevN).normalized();
		}
		else {
			N = prevN;
		}
		B = QVector3D::crossProduct(T, N).normalized();

		prevT = T;
		prevN = N;

		//生成截面点
		for (int j = 0; j < segmentPointsCount; ++j) {
			float angle = j * twoPi / segmentPointsCount;
			QVector3D offset = N * cos(angle) * radius + B * sin(angle) * radius;
			QVector3D vertex = curvePoints[i] + offset;
			outVertices.push_back(vertex);
			outNormals.push_back(offset.normalized());
		}
	}

	//生成索引
	for (int i = 0; i < n - 2; ++i) {
		int start0 = i * segmentPointsCount;
		int start1 = (i + 1) * segmentPointsCount;
		for (int j = 0; j < segmentPointsCount; ++j) {
			int curr = j;
			int next = (j + 1) % segmentPointsCount;
			outIndices.push_back(start0 + curr);
			outIndices.push_back(start1 + curr);
			outIndices.push_back(start1 + next);

			outIndices.push_back(start0 + curr);
			outIndices.push_back(start1 + next);
			outIndices.push_back(start0 + next);
		}
	}

	//绘制
	unsigned int vao, vbo, ebo;
	m_QOpenGLFunction->glGenVertexArrays(1, &vao);
	m_QOpenGLFunction->glBindVertexArray(vao);

	m_QOpenGLFunction->glGenBuffers(1, &vbo);
	m_QOpenGLFunction->glBindBuffer(GL_ARRAY_BUFFER, vbo);
	m_QOpenGLFunction->glBufferData(GL_ARRAY_BUFFER, outVertices.size() * sizeof(QVector3D), outVertices.data(), GL_STATIC_DRAW);

	m_QOpenGLFunction->glGenBuffers(1, &ebo);
	m_QOpenGLFunction->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	m_QOpenGLFunction->glBufferData(GL_ELEMENT_ARRAY_BUFFER, outIndices.size() * sizeof(unsigned int), outIndices.data(), GL_STATIC_DRAW);

	m_QOpenGLFunction->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)0);
	m_QOpenGLFunction->glEnableVertexAttribArray(0);

	m_LineShader->bind();
	//模型矩阵
	//m_Shader->setUniformValue("model", model);
	//观察矩阵
	m_LineShader->setUniformValue("view", view);
	//投影矩阵
	m_LineShader->setUniformValue("projection", projection);
	//m_QOpenGLFunction->glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	m_QOpenGLFunction->glDrawElements(GL_TRIANGLES, outIndices.size(), GL_UNSIGNED_INT, nullptr);
	m_LineShader->release();

}