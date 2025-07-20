#include "MyOpenGLWidget.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <Mark.h>
#include <Model.h>
#include <ModelLine.h>

MyOpenGLWidget::MyOpenGLWidget(QWidget* parent) :QOpenGLWidget(parent),
m_Shader(nullptr),
m_CameraPos(0.0f, 0.0f, 3.0f),
m_CameraTarget(0.0f, 0.0f, 0.0f),
m_Up(0.0f, 1.0f, 0.0f)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setFocusPolicy(Qt::StrongFocus);
	setAttribute(Qt::WA_OpaquePaintEvent);  // 确保事件不会被遮挡
	setMouseTracking(true);
}
MyOpenGLWidget::~MyOpenGLWidget()
{
	delete m_Shader;
	delete m_Mark;
	delete m_ParametricModeling;
	delete m_PickShader;
}

void MyOpenGLWidget::initializeGL() {
	//初始化
	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//// 启用混合,用来让包围盒透明
	//glEnable(GL_BLEND);
	//// 设置混合函数
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//禁止背面剔除
	//glDisable(GL_CULL_FACE);
	// 启用线框模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//初始化几个变化矩阵
	//模型矩阵
	m_MatrixModel.translate(QVector3D(0.0f, 0.0f, 0.0f));
	//m_MatrixModel.scale(QVector3D(1.0f, 1.0f, 1.0f));
	//m_MatrixModel.rotate(45.0, QVector3D(1.0f, 0.0f, 0.0f));
	//m_MatrixModel.rotate(45.0, QVector3D(0.0f, 1.0f, 0.0f));
	//观察矩阵
	m_MatrixView.lookAt(m_CameraPos, m_CameraTarget, m_Up);
	//投影矩阵
	m_MatrixProjection.perspective(45.0, (float)this->width() / (float)this->height(), 0.1, 100.0);

	m_Shader = new QOpenGLShaderProgram(this);
	if (!m_Shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/VertexShader.glsl")) {
		qDebug() << "Vertex shader error:" << m_Shader->log();
	}
	if (!m_Shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/FragmentShader.glsl")) {
		qDebug() << "Fragment shader error:" << m_Shader->log();
	}
	if (!m_Shader->link()) {
		qDebug() << "Shader program link error:" << m_Shader->log();
	}

	m_PickShader = new QOpenGLShaderProgram(this);
	if (!m_PickShader->addShaderFromSourceFile(QOpenGLShader::Compute, "./Shaders/Picking.vert")) {
		qDebug() << "Picking shader error:" << m_PickShader->log();
	}
	if (!m_PickShader->link()) {
		qDebug() << "Picking shader program link error:" << m_PickShader->log();
	}

	m_TestShader = new QOpenGLShaderProgram(this);
	if (!m_TestShader->addShaderFromSourceFile(QOpenGLShader::Compute, "./Shaders/test.vert")) {
		qDebug() << "Picking shader error:" << m_TestShader->log();
	}
	if (!m_TestShader->link()) {
		qDebug() << "Picking shader program link error:" << m_TestShader->log();
	}

	m_QOpengGlFunction = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_5_Core>(QOpenGLContext::currentContext());



	////有法向量的顶点数据
	//float vertices[] = {
	//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	//	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	//	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	//	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	//	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	//	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	//	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	//	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	//	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	//	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	//	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	//	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	//	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	//	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	//	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	//	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	//	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	//};

	//Element element;
	//int count = sizeof(vertices) / sizeof(float);
	//for (int i = 0; i < count; i += 8) {
	//	Bim::Vertex vertex;
	//	vertex.Position = QVector3D(vertices[i], vertices[i + 1], vertices[i + 2]);
	//	vertex.Normal = QVector3D(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
	//	vertex.TexCoords = QVector2D(vertices[i + 6], vertices[i + 7]);
	//	element.mesh.vertices.push_back(vertex);
	//}
	//Elements.push_back(element);

	//for (auto ele: Elements) {
	//	m_QOpengGlFunction->glGenVertexArrays(1, &VAO);
	//	m_QOpengGlFunction->glBindVertexArray(VAO);

	//	m_QOpengGlFunction->glGenBuffers(1, &VBO);
	//	m_QOpengGlFunction->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	m_QOpengGlFunction->glBufferData(GL_ARRAY_BUFFER, ele.mesh.vertices.size() * sizeof(Vertex), ele.mesh.vertices.data(), GL_STATIC_DRAW);
	//	qDebug() << m_QOpengGlFunction->glGetError();

	//	//m_QOpengGlFunction->glGenBuffers(1, &EBO);
	//	//m_QOpengGlFunction->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//	//m_QOpengGlFunction->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	//	//qDebug() << m_QOpengGlFunction->glGetError();

	//	// vertex Positions
	//	m_QOpengGlFunction->glEnableVertexAttribArray(0);
	//	m_QOpengGlFunction->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	//	qDebug() << m_QOpengGlFunction->glGetError();
	//	// vertex normals
	//	m_QOpengGlFunction->glEnableVertexAttribArray(1);
	//	m_QOpengGlFunction->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//	qDebug() << m_QOpengGlFunction->glGetError();
	//	// vertex texture coords
	//	m_QOpengGlFunction->glEnableVertexAttribArray(2);
	//	m_QOpengGlFunction->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	//	qDebug() << m_QOpengGlFunction->glGetError();
	//}

	//m_Model = new Model(func);
	//m_Model->LoadFile("C:/MyProject/LearnOpenGL/Resources/labixiaoxin/62b01271ee64be39728ffda6d1a6f53a.obj");
	//m_Model->LoadFile("C:/MyProject/LearnOpenGL/x64/Debug/Resources/backpack/backpack.obj");
	//m_Mark = new Mark(func);
	//m_ParametricModeling = new ParametricModeling(func);
	//m_ModelLine = std::make_shared<ModelLine>(m_QOpengGlFunction);
	//m_BasicPrimitives = std::make_shared<BasicPrimitives>(m_QOpengGlFunction);

};
void MyOpenGLWidget::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);
	//投影矩阵
	m_MatrixProjection.setToIdentity();
	m_MatrixProjection.perspective(45.0, (float)w / (float)h, 0.1, 100.0);
};
void MyOpenGLWidget::paintGL() {
	//清除颜色和深度缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2, 0.2, 0.2, 1.0);

	if (!mesh_inited && file_loader != nullptr) {
		InitMesh();
		ResetCameraToFitModel(file_loader->elements);
		//UploadTriangleData();
	}

	m_Shader->bind();
	m_Shader->setUniformValue("model", m_MatrixModel);
	m_Shader->setUniformValue("view", m_MatrixView);
	m_Shader->setUniformValue("projection", m_MatrixProjection);
	m_Shader->setUniformValue("is_selected", false);
	m_Shader->setUniformValue("u_clipPlane", m_ClipPlane);

	if (file_loader != nullptr) {
		qDebug() << "draw glfw " << file_loader->elements.size();

		for (auto& element : file_loader->elements)
		{
			element.mesh.Draw(*m_Shader);
		}
	}
	m_Shader->release();


	// 2. 绘制选中的模型（稍微向前偏移）

	m_Shader->bind();
	m_Shader->setUniformValue("model", m_MatrixModel);
	m_Shader->setUniformValue("view", m_MatrixView);
	m_Shader->setUniformValue("projection", m_MatrixProjection);
	m_Shader->setUniformValue("is_selected", true);

	if (file_loader != nullptr) {
		for (auto& element : file_loader->elements) {
			if (element.objectId == selected_object_id) {
				element.mesh.Draw(*m_Shader);
				break;
			}
		}
	}
	m_Shader->release();

	// 3. 绘制线框（最顶层）

	if (m_ModelLine) {
		m_ModelLine->Draw(m_MatrixModel, m_MatrixView, m_MatrixProjection);
		MarkText();
		MarkAngle();
	}

	//if (m_Model->IsShow) {
	//	m_Shader->bind();
	//	m_Shader->setUniformValue("model", m_MatrixModel);

	//	////观察举证
	//	if (m_Model->IsSelected) {
	//		QMatrix4x4 view;
	//		QVector3D modelCenter = (m_Model->BindingBox.Min + m_Model->BindingBox.Max) / 2;
	//		modelCenter = m_MatrixModel.map(modelCenter);
	//		float radius = (m_Model->BindingBox.Max - m_Model->BindingBox.Min).length() * 0.5f;
	//		float distance = radius / std::tan(qDegreesToRadians(45.0f * 0.5f));
	//		distance *= 1.5f; // 适当拉远一点，防止模型太满
	//		QVector3D cameraDir = QVector3D(0, 0, 1);
	//		QVector3D cameraPos = modelCenter + cameraDir * distance;
	//		QVector3D up(0.0f, 1.0f, 0.0f);
	//		view.lookAt(cameraPos, modelCenter, up);
	//		m_Shader->setUniformValue("view", view);
	//	}
	//	else {
	//		m_Shader->setUniformValue("view", m_MatrixView);
	//	}
	//	m_Shader->setUniformValue("projection", m_MatrixProjection);

	//	//是否选中，选中则高亮
	//	m_Shader->setUniformValue("isSelected", m_Model->IsSelected);

	//	m_Model->Draw(*m_Shader);
	//	m_Shader->release();
	//	//绘制标注线
	//	//m_Mark->DrawLine(width(), height(), m_MatrixModel, m_MatrixView, m_MatrixProjection);
	//	//绘制贝塞尔曲线
	//	//m_ParametricModeling->BezierCurves(100, m_MatrixModel, m_MatrixView, m_MatrixProjection);
	//}
	//m_Model->ShowBindingBox(this->width(), this->height(), *m_CubeShader, m_MatrixModel, m_MatrixView, m_MatrixProjection);
	//m_Mark->DrawTxt("demo", 25, 25, 1.0f, { 1.0f, 0.1f, 0.1f }, width(), height());

	//if (m_ModelLine != nullptr) {
	//	m_ModelLine->BSpline(m_ModelLine->Vertices, m_MatrixModel, m_MatrixView, m_MatrixProjection);
	//}

	//if (m_BasicPrimitives != nullptr) {
	//	m_BasicPrimitives->Ball();
	//}

};

void MyOpenGLWidget::mousePressEvent(QMouseEvent* event) {
	//Test();
	PerformPicking(event->pos());
	QVector3D p = ScreenToWorld(event->pos().x(), event->pos().y());
	//m_Mark->LinePoints.push_back(p);
	//m_ParametricModeling->ControlPoints.push_back(p);

	//获取点击的射线，选中模型
	//QVector3D rayDir = ScreenPosToRayDir(event->pos().x(), event->pos().y());
	//QMatrix4x4 invModel = m_MatrixModel.inverted();
	//QVector3D localOrigin = invModel.map(cameraPos);
	//QVector3D localDir = (invModel.map(cameraPos + rayDir) - localOrigin).normalized();
	//float tMin, tMax;
	//if (RayIntersectsAABB(localOrigin, localDir, m_Model->BindingBox.Min, m_Model->BindingBox.Max, tMin, tMax)) {
	//	m_Model->IsSelected = true;
	//}
	//else {
	//	m_Model->IsSelected = false;
	//}
	//进入视图旋转模式
	if (event->button() == Qt::RightButton) {
		m_IsRightMousePress = true;
		m_RightMousePoint = event->pos();
	}
	//绘制直线
	if (CommandMode == CommandMode::ModelLine && event->button() == Qt::LeftButton) {
		Vertex vertex;
		vertex.Position = p;
		m_ModelLine->element.mesh.vertices.push_back(vertex);
	}
	//绘制b样条曲线
	if (CommandMode == CommandMode::BSpline && event->button() == Qt::LeftButton) {
		//m_ModelLine->Vertices.push_back(p);
		if (m_ModelLine) {
			Vertex vertex;
			vertex.Position = p;
			m_ModelLine->element.mesh.vertices.push_back(vertex);
		}
	}
	//绘制球
	if (CommandMode == CommandMode::Ball && event->button() == Qt::LeftButton) {
		m_BasicPrimitives->offset = p;
	}

	update();
}

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton) {
		m_IsRightMousePress = false;
		m_RightMousePoint = event->pos();
	}

	if (CommandMode == CommandMode::ModelLine && event->button() == Qt::LeftButton) {
		//绘制模型线
		//m_ModelLine->Vertices.push_back(ScreenToWorld(event->pos().x(), event->pos().y()));
	}
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
	if (m_IsAltPress && m_IsRightMousePress) {
		QPoint delta = event->pos() - m_RightMousePoint;
		float sensitivity = 0.005f;
		m_Yaw += delta.x() * sensitivity;
		m_Pitch += -delta.y() * sensitivity;
		m_Pitch = clamp(m_Pitch, -1.55f, 1.55f);  // 限制角度
		QVector3D position;
		position.setX(m_CameraTarget.x() + m_Distance * cos(m_Pitch) * sin(m_Yaw));
		position.setY(m_CameraTarget.y() + m_Distance * sin(m_Pitch));
		position.setZ(m_CameraTarget.z() + m_Distance * cos(m_Pitch) * cos(m_Yaw));
		m_MatrixView.setToIdentity();
		m_MatrixView.lookAt(position, m_CameraTarget, m_Up);
		m_RightMousePoint = event->pos();
	}
	if (CommandMode == CommandMode::ModelLine) {
		m_ModelLine->last_vertex.Position = ScreenToWorld(event->pos().x(), event->pos().y());
		qDebug() << m_ModelLine->last_vertex.Position.x() << m_ModelLine->last_vertex.Position.y() << m_ModelLine->last_vertex.Position.z();
	}
	update();
}

void MyOpenGLWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Alt) {
		m_IsAltPress = true;
	}
}
void MyOpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Alt) {
		m_IsAltPress = false;
	}
}
QVector3D MyOpenGLWidget::ScreenToWorld(int x, int y) {
	float ndcX = (2.0f * x) / width() - 1.0f;
	float ndcY = 1.0f - (2.0f * y) / height();

	QVector4D nearPoint(ndcX, ndcY, -1.0f, 1.0f);
	QVector4D farPoint(ndcX, ndcY, 1.0f, 1.0f);

	//逆矩阵
	QMatrix4x4 inv = (m_MatrixProjection * m_MatrixView).inverted();

	QVector4D worldNear = inv * nearPoint;
	QVector4D worldFar = inv * farPoint;

	worldNear /= worldNear.w();
	worldFar /= worldFar.w();

	//构造射线
	QVector3D rayOrigin = worldNear.toVector3D();
	QVector3D rayDir = (worldFar - worldNear).toVector3D().normalized();

	float t = -rayOrigin.z() / rayDir.z();
	QVector3D pointOnPlane = rayOrigin + t * rayDir;
	return pointOnPlane;
}
/// <summary>
/// 判断鼠标点击是否与模型的包围盒相交
/// </summary>
/// <returns></returns>
bool MyOpenGLWidget::RayIntersectsAABB(const QVector3D& rayOrigin,
	const QVector3D& rayDir,
	const QVector3D& boxMin,
	const QVector3D& boxMax,
	float& tmin,
	float& tmax) {
	tmin = -std::numeric_limits<float>::infinity();
	tmax = std::numeric_limits<float>::infinity();

	for (int i = 0; i < 3; ++i) {
		float invD = 1.0 / rayDir[i];
		float t0 = (boxMin[i] - rayOrigin[i]) * invD;
		float t1 = (boxMax[i] - rayOrigin[i]) * invD;
		if (invD < 0.0f) std::swap(t0, t1);
		tmin = std::max(tmin, t0);
		tmax = std::min(tmax, t1);
		if (tmax < tmin) {
			return false;
		}
	}
	return true;
}
/// <summary>
/// 射线
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
QVector3D MyOpenGLWidget::ScreenPosToRayDir(int x, int y) {
	float ndcX = (2.0f * x) / width() - 1.0f;
	float ndcY = 1.0f - (2.0f * y) / height();
	QVector4D rayClip(ndcX, ndcY, -1.0, 1.0);

	QMatrix4x4 invProjView = (m_MatrixProjection * m_MatrixView).inverted();
	QVector4D rayWorld = invProjView * rayClip;
	rayWorld /= rayWorld.w();

	QVector3D cameraPos = m_MatrixView.inverted().column(3).toVector3D();
	QVector3D dir = (rayWorld.toVector3D() - cameraPos).normalized();
	return dir;
}
void MyOpenGLWidget::wheelEvent(QWheelEvent* event) {
	float delta = event->angleDelta().y() / 120.0f; // 每次滚轮滚动的增量
	float zoomFactor = 0.2f;
	m_Distance -= delta * zoomFactor;
	QVector3D position;
	position.setX(m_CameraTarget.x() + m_Distance * cos(m_Pitch) * sin(m_Yaw));
	position.setY(m_CameraTarget.y() + m_Distance * sin(m_Pitch));
	position.setZ(m_CameraTarget.z() + m_Distance * cos(m_Pitch) * cos(m_Yaw));
	m_MatrixView.setToIdentity();
	m_MatrixView.lookAt(position, m_CameraTarget, m_Up);
	update();
}
void MyOpenGLWidget::Scale() {

}
void MyOpenGLWidget::CreateModelLine() {
	makeCurrent(); // 激活上下文
	m_ModelLine = std::make_shared<ModelLine>(m_QOpengGlFunction);
}

void MyOpenGLWidget::InitMesh() {
	for (auto& element : file_loader->elements) {
		element.mesh.m_QOpengGlFunction = m_QOpengGlFunction;
		element.mesh.SetupMesh();
		mesh_inited = true;
	}

	//for (auto &ele: Elements) {
	//	glGenVertexArrays(1, &VAO);
	//	glBindVertexArray(VAO);

	//	glGenBuffers(1, &VBO);
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	glBufferData(GL_ARRAY_BUFFER, ele.mesh.vertices.size() * sizeof(Vertex), ele.mesh.vertices.data(), GL_STATIC_DRAW);
	//	qDebug() << glGetError();

	//	//m_QOpengGlFunction->glGenBuffers(1, &EBO);
	//	//m_QOpengGlFunction->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//	//m_QOpengGlFunction->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	//	//qDebug() << m_QOpengGlFunction->glGetError();

	//	// vertex Positions
	//	glEnableVertexAttribArray(0);
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	//	qDebug() << glGetError();
	//	// vertex normals
	//	glEnableVertexAttribArray(1);
	//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//	qDebug() << glGetError();
	//	// vertex texture coords
	//	glEnableVertexAttribArray(2);
	//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	//	qDebug() << glGetError();

	//	//m_QOpengGlFunction->glGenVertexArrays(1, &VAO);
	//	//m_QOpengGlFunction->glBindVertexArray(VAO);

	//	//m_QOpengGlFunction->glGenBuffers(1, &VBO);
	//	//m_QOpengGlFunction->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	//m_QOpengGlFunction->glBufferData(GL_ARRAY_BUFFER, ele.mesh.vertices.size() * sizeof(Vertex), ele.mesh.vertices.data(), GL_STATIC_DRAW);
	//	//qDebug() << m_QOpengGlFunction->glGetError();

	//	////m_QOpengGlFunction->glGenBuffers(1, &EBO);
	//	////m_QOpengGlFunction->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//	////m_QOpengGlFunction->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	//	////qDebug() << m_QOpengGlFunction->glGetError();

	//	//// vertex Positions
	//	//m_QOpengGlFunction->glEnableVertexAttribArray(0);
	//	//m_QOpengGlFunction->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	//	//qDebug() << m_QOpengGlFunction->glGetError();
	//	//// vertex normals
	//	//m_QOpengGlFunction->glEnableVertexAttribArray(1);
	//	//m_QOpengGlFunction->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//	//qDebug() << m_QOpengGlFunction->glGetError();
	//	//// vertex texture coords
	//	//m_QOpengGlFunction->glEnableVertexAttribArray(2);
	//	//m_QOpengGlFunction->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	//	//qDebug() << m_QOpengGlFunction->glGetError();

	//	//m_FileLoaded = true;
	//}
	//m_FileLoaded = true;
}

/// <summary>
/// 模型加载时，相机适应
/// </summary>
void MyOpenGLWidget::ResetCameraToFitModel(const std::vector<Element>& elements) {
	QVector3D minPos(FLT_MAX, FLT_MAX, FLT_MAX);
	QVector3D maxPos(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (const auto& element : elements) {
		for (const auto& vertex : element.mesh.vertices) {
			const QVector3D& pos = vertex.Position;
			minPos.setX(std::min(minPos.x(), pos.x()));
			minPos.setY(std::min(minPos.y(), pos.y()));
			minPos.setZ(std::min(minPos.z(), pos.z()));
			maxPos.setX(std::max(maxPos.x(), pos.x()));
			maxPos.setY(std::max(maxPos.y(), pos.y()));
			maxPos.setZ(std::max(maxPos.z(), pos.z()));
		}
	}
	float radius = (maxPos - minPos).length() * 0.5f;
	QVector3D cameraPos = m_CameraTarget + QVector3D(0, 0, radius * 2.5f);
	m_Distance = (cameraPos - m_CameraTarget).length();
	m_MatrixView.setToIdentity();
	m_MatrixView.lookAt(cameraPos, m_CameraTarget, m_Up);
}
/// <summary>
/// 上传三角面数据到gpu
/// </summary>
void MyOpenGLWidget::UploadTriangleData() {
	glGenBuffers(1, &m_TriangleSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_TriangleSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, file_loader->triangles.size() * sizeof(GPUTriangle), file_loader->triangles.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_TriangleSSBO);
}
/// <summary>
/// 拾取
/// </summary>
/// <param name="mousePos"></param>
void MyOpenGLWidget::PerformPicking(const QPoint& mousePos) {
	if (file_loader == nullptr) return;

	//上传三角形数据
	unsigned int trianglessbo;
	glGenBuffers(1, &trianglessbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, trianglessbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, file_loader->triangles.size() * sizeof(GPUTriangle), file_loader->triangles.data(), GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, trianglessbo);

	//上传射线数据
	QVector3D origin = ScreenToWorld(mousePos.x(), mousePos.y());
	QVector3D rayDir = ScreenPosToRayDir(mousePos.x(), mousePos.y());
	GPURay ray;
	ray.origin = glm::vec4(origin.x(), origin.y(), origin.z(), 1.0);
	ray.direction = glm::vec4(rayDir.x(), rayDir.y(), rayDir.z(), 0.0);
	unsigned int rayssbo;
	glGenBuffers(1, &rayssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rayssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GPURay), &ray, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, rayssbo);

	// 3. 初始化输出结果
	GPUResult result = { -1, 1e9f };
	unsigned int resultssbo;
	glGenBuffers(1, &resultssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, resultssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GPUResult), &result, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, resultssbo);

	GLuint debugSSBO;
	glGenBuffers(1, &debugSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, debugSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, file_loader->triangles.size() * 4 * sizeof(int), nullptr, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, debugSSBO);

	// 执行 compute shader
	m_TestShader->bind();
	GLuint workGroupCount = (file_loader->triangles.size() + 255) / 256;
	glDispatchCompute(workGroupCount, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, resultssbo);
	GPUResult* resultData = (GPUResult*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	if (resultData) {
		qDebug() << resultData->objectId;
		if (resultData->objectId != -1) {
			selected_object_id = resultData->objectId;
			/*for (auto& element : file_loader->elements)
			{
				element.is_selected = true;
			}*/
		}
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	// 读取数据
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, debugSSBO);
	int* data = (int*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, file_loader->triangles.size() * 4 * sizeof(int), GL_MAP_READ_BIT);
	if (data) {
		for (int i = 0; i < 5; ++i) {
			//printf("idx = %d -> %d, %d, %d, %d\n", i, data[i * 4 + 0], data[i * 4 + 1], data[i * 4 + 2], data[i * 4 + 3]);
			qDebug() << data[i * 4 + 0] << data[i * 4 + 1] << data[i * 4 + 2] << data[i * 4 + 3];
		}
	}
	else {
		printf("glMapBufferRange failed.\n");
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glDeleteBuffers(1, &debugSSBO);
	m_TestShader->release();
}

void MyOpenGLWidget::Test() {
	int triangleCount = 2;  // 可测试较小的值

	std::vector<GPUTriangle> sceneTriangles;

	GPUTriangle triangle;
	triangle.v0 = glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
	triangle.v1 = glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
	triangle.v2 = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	triangle.objectId = 1;
	sceneTriangles.push_back(triangle);

	GPUTriangle triangle1;
	triangle1.v0 = glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
	triangle1.v1 = glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
	triangle1.v2 = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	triangle1.objectId = 2;
	sceneTriangles.push_back(triangle1);

	GPUTriangle triangle2;
	triangle2.v0 = glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
	triangle2.v1 = glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
	triangle2.v2 = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	triangle2.objectId = 3;
	sceneTriangles.push_back(triangle2);

	int debugBufferSize = sceneTriangles.size() * 4 * sizeof(int);

	unsigned int trianglessbo;
	glGenBuffers(1, &trianglessbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, trianglessbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sceneTriangles.size() * sizeof(GPUTriangle), sceneTriangles.data(), GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, trianglessbo);

	GLuint debugSSBO;
	glGenBuffers(1, &debugSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, debugSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, debugBufferSize, nullptr, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, debugSSBO);

	// 执行 compute shader
	m_TestShader->bind();
	GLuint workGroupCount = (triangleCount + 255) / 256;
	glDispatchCompute(workGroupCount, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	// 读取数据
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, debugSSBO);
	int* data = (int*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, debugBufferSize, GL_MAP_READ_BIT);
	if (data) {
		for (int i = 0; i < 5; ++i) {
			//printf("idx = %d -> %d, %d, %d, %d\n", i, data[i * 4 + 0], data[i * 4 + 1], data[i * 4 + 2], data[i * 4 + 3]);
			qDebug() << data[i * 4 + 0] << data[i * 4 + 1] << data[i * 4 + 2] << data[i * 4 + 3];
		}
	}
	else {
		printf("glMapBufferRange failed.\n");
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	glDeleteBuffers(1, &debugSSBO);
	m_TestShader->release();
}
/// <summary>
/// 二位文字标注
/// </summary>
void MyOpenGLWidget::MarkText() {
	if (m_ModelLine != nullptr) {
		if (m_ModelLine->element.mesh.vertices.size() < 2) return;
		for (int i = 0; i < m_ModelLine->element.mesh.vertices.size() - 1; ++i) {
			QVector3D p1 = m_ModelLine->element.mesh.vertices[i].Position;
			QVector3D p2 = m_ModelLine->element.mesh.vertices[i + 1].Position;
			QVector3D mid = (p1 + p2) / 2.0f;
			float length = (p2 - p1).length();
			QMatrix4x4 mvp = m_MatrixProjection * m_MatrixView * m_MatrixModel;
			QVector4D clipPos = mvp * QVector4D(mid, 1.0);
			clipPos /= clipPos.w(); // 归一化设备坐标

			// 转为窗口坐标
			float x = (clipPos.x() * 0.5f + 0.5f) * width();
			float y = (1.0f - (clipPos.y() * 0.5f + 0.5f)) * height(); // y 轴倒置

			QString text = QString::number(length, 'f', 2) + " m";
			QPainter painter(this);
			painter.setPen(Qt::white);
			painter.setFont(QFont("Arial", 12));
			painter.drawText(QPointF(x, y), text);
			painter.end();

		}
	}
}
/// <summary>
/// 角度标注
/// </summary>
void MyOpenGLWidget::MarkAngle() {
	if (m_ModelLine != nullptr) {
		if (m_ModelLine->element.mesh.vertices.size() < 3) return;
		for (int i = 0; i < m_ModelLine->element.mesh.vertices.size() - 2; ++i) {
			QVector3D p1 = m_ModelLine->element.mesh.vertices[i].Position;
			QVector3D p2 = m_ModelLine->element.mesh.vertices[i + 1].Position;
			QVector3D p3 = m_ModelLine->element.mesh.vertices[i + 2].Position;
			QMatrix4x4 mvp = m_MatrixProjection * m_MatrixView * m_MatrixModel;
			QVector4D clipPos = mvp * QVector4D(p2, 1.0);
			clipPos /= clipPos.w(); // 归一化设备坐标

			// 转为窗口坐标
			float x = (clipPos.x() * 0.5f + 0.5f) * width();
			float y = (1.0f - (clipPos.y() * 0.5f + 0.5f)) * height(); // y 轴倒置

			//求角度
			float dotvalue = QVector3D::dotProduct(p1 - p2, p3 - p2);
			float len1 = (p1 - p2).length();
			float len2 = (p3 - p2).length();
			// 防止除以0
			if (len1 < 1e-6f || len2 < 1e-6f)
				return;
			float cosTheta = dotvalue / (len1 * len2);
			// 限制在 -1~1 之间，防止数值误差导致 acos 出 NaN
			cosTheta = qBound(-1.0f, cosTheta, 1.0f);
			float radians = qAcos(cosTheta); // 返回的是弧度
			float angle = qRadiansToDegrees(radians); // 转为角度
			QString text = QString::number(angle, 'f', 2) + QString::fromUtf8("°");
			QPainter painter(this);
			painter.setPen(Qt::white);
			painter.setFont(QFont("Arial", 12));
			painter.drawText(QPointF(x, y), text);
			painter.end();

		}
	}
}
