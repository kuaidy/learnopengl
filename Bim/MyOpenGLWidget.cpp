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
	setFocusPolicy(Qt::StrongFocus);
}
MyOpenGLWidget::~MyOpenGLWidget()
{
	delete m_Shader;
	delete m_Mark;
	delete m_ParametricModeling;
}

void MyOpenGLWidget::initializeGL() {
	//��ʼ��
	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// ���û��,�����ð�Χ��͸��
	glEnable(GL_BLEND);
	// ���û�Ϻ���
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//��ֹ�����޳�
	glDisable(GL_CULL_FACE);
	// �����߿�ģʽ
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//��ʼ�������仯����
	//ģ�;���
	m_MatrixModel.translate(QVector3D(0.0f, 0.0f, 0.0f));
	m_MatrixModel.scale(QVector3D(1.0f, 1.0f, 1.0f));
	//m_MatrixModel.rotate(45.0, QVector3D(1.0f, 0.0f, 0.0f));
	m_MatrixModel.rotate(45.0, QVector3D(0.0f, 1.0f, 0.0f));
	//�۲����
	m_MatrixView.lookAt(m_CameraPos, m_CameraTarget, m_Up);
	//ͶӰ����
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
	m_QOpengGlFunction = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_5_Core>(QOpenGLContext::currentContext());
	//m_Model = new Model(func);
	//m_Model->LoadFile("C:/MyProject/LearnOpenGL/Resources/labixiaoxin/62b01271ee64be39728ffda6d1a6f53a.obj");
	//m_Model->LoadFile("C:/MyProject/LearnOpenGL/x64/Debug/Resources/backpack/backpack.obj");
	//m_Mark = new Mark(func);
	//m_ParametricModeling = new ParametricModeling(func);
	m_ModelLine = std::make_shared<ModelLine>(m_QOpengGlFunction);
	m_BasicPrimitives = std::make_shared<BasicPrimitives>(m_QOpengGlFunction);

};
void MyOpenGLWidget::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);
};
void MyOpenGLWidget::paintGL() {
	//�����ɫ����Ȼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2, 0.2, 0.2, 1.0);

	m_Shader->bind();
	m_Shader->setUniformValue("model", m_MatrixModel);
	m_Shader->setUniformValue("view", m_MatrixView);
	m_Shader->setUniformValue("projection", m_MatrixProjection);

	if (m_FileLoaded) {
		for (int i = 0; i < Elements.size(); ++i) {
			Elements[i].mesh.Draw(*m_Shader);
		}
	}
	m_Shader->release();

	//if (m_Model->IsShow) {
	//	m_Shader->bind();
	//	m_Shader->setUniformValue("model", m_MatrixModel);

	//	////�۲��֤
	//	if (m_Model->IsSelected) {
	//		QMatrix4x4 view;
	//		QVector3D modelCenter = (m_Model->BindingBox.Min + m_Model->BindingBox.Max) / 2;
	//		modelCenter = m_MatrixModel.map(modelCenter);
	//		float radius = (m_Model->BindingBox.Max - m_Model->BindingBox.Min).length() * 0.5f;
	//		float distance = radius / std::tan(qDegreesToRadians(45.0f * 0.5f));
	//		distance *= 1.5f; // �ʵ���Զһ�㣬��ֹģ��̫��
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

	//	//�Ƿ�ѡ�У�ѡ�������
	//	m_Shader->setUniformValue("isSelected", m_Model->IsSelected);

	//	m_Model->Draw(*m_Shader);
	//	m_Shader->release();
	//	//���Ʊ�ע��
	//	//m_Mark->DrawLine(width(), height(), m_MatrixModel, m_MatrixView, m_MatrixProjection);
	//	//���Ʊ���������
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
	QVector3D p = ScreenToWorld(event->pos().x(), event->pos().y());
	//m_Mark->LinePoints.push_back(p);
	//m_ParametricModeling->ControlPoints.push_back(p);

	//��ȡ��������ߣ�ѡ��ģ��
	QVector3D cameraPos(0.0f, 0.0f, 3.0f);
	QVector3D rayDir = ScreenPosToRayDir(event->pos().x(), event->pos().y());
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
	//������ͼ��תģʽ
	if (event->button() == Qt::RightButton) {
		m_IsRightMousePress = true;
		m_RightMousePoint = event->pos();
	}
	//����b��������
	if (CommandMode == CommandMode::BSpline && event->button() == Qt::LeftButton) {
		m_ModelLine->Vertices.push_back(p);
	}
	//������
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
		//����ģ����
		m_ModelLine->Vertices.push_back(ScreenToWorld(event->pos().x(), event->pos().y()));
	}
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
	if (m_IsAltPress && m_IsRightMousePress) {
		QPoint delta = event->pos() - m_RightMousePoint;
		float sensitivity = 0.005f;
		m_Yaw += delta.x() * sensitivity;
		m_Pitch += -delta.y() * sensitivity;
		m_Pitch = clamp(m_Pitch, -1.55f, 1.55f);  // ���ƽǶ�
		QVector3D position;
		position.setX(m_CameraTarget.x() + m_Distance * cos(m_Pitch) * sin(m_Yaw));
		position.setY(m_CameraTarget.y() + m_Distance * sin(m_Pitch));
		position.setZ(m_CameraTarget.z() + m_Distance * cos(m_Pitch) * cos(m_Yaw));
		m_MatrixView.setToIdentity();
		m_MatrixView.lookAt(position, m_CameraTarget, m_Up);
		m_RightMousePoint = event->pos();
	}
	if (CommandMode == CommandMode::ModelLine) {
		m_ModelLine->LastVertex = ScreenToWorld(event->pos().x(), event->pos().y());
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

	//�����
	QMatrix4x4 inv = (m_MatrixProjection * m_MatrixView).inverted();

	QVector4D worldNear = inv * nearPoint;
	QVector4D worldFar = inv * farPoint;

	worldNear /= worldNear.w();
	worldFar /= worldFar.w();

	//��������
	QVector3D rayOrigin = worldNear.toVector3D();
	QVector3D rayDir = (worldFar - worldNear).toVector3D().normalized();

	float t = -rayOrigin.z() / rayDir.z();
	QVector3D pointOnPlane = rayOrigin + t * rayDir;
	return pointOnPlane;
}
/// <summary>
/// �ж�������Ƿ���ģ�͵İ�Χ���ཻ
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
/// ����
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
	float delta = event->angleDelta().y() / 120.0f; // ÿ�ι��ֹ���������
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
	m_ModelLine = std::make_shared<ModelLine>(m_QOpengGlFunction);
}

void MyOpenGLWidget::InitMesh() {
	for (auto& element : Elements) {
		element.mesh.m_QOpengGlFunction = m_QOpengGlFunction;
		element.mesh.SetupMesh();
		m_FileLoaded = true;
	}
}