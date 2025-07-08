#include "BimMainWindow.h"

BimMainWindow::BimMainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_MyOpenGlWidget = new MyOpenGLWidget(ui.frameopengl);
	// ���ò���
	QVBoxLayout* layout = new QVBoxLayout(ui.frameopengl);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_MyOpenGlWidget);
	// ��ѡ�����ô�С����
	m_MyOpenGlWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

BimMainWindow::~BimMainWindow()
{
}

void BimMainWindow::OpenModule(string path) {

}

void BimMainWindow::on_fileopen_triggered() {
	QString fileName = QFileDialog::getOpenFileName(
		this,
		tr("ѡ��ģ���ļ�"),
		"",
		tr("ģ���ļ�(*.gltf *.glb *.obj *.fbx *.stl *.ifc);;�����ļ� (*.*)")
	);
	std::unique_ptr<FileLoader> fileLoader = FileLoadFactory::Create(fileName.toStdString());
	m_MyOpenGlWidget->Elements = fileLoader->Load(fileName.toStdString());
	m_MyOpenGlWidget->InitMesh();
}
void BimMainWindow::on_modelline_triggered(bool flag) {
	if (flag) {
		m_MyOpenGlWidget->CommandMode = CommandMode::ModelLine;
		m_MyOpenGlWidget->CreateModelLine();
	}
	else {
		m_MyOpenGlWidget->CommandMode = CommandMode::None;
	}
}
/// <summary>
/// ����b��������
/// </summary>
/// <param name="flag"></param>
void BimMainWindow::on_bspline_triggered(bool flag) {
	if (flag) {
		m_MyOpenGlWidget->CommandMode = CommandMode::BSpline;
	}
	else {
		m_MyOpenGlWidget->CommandMode = CommandMode::None;
	}
}
/// <summary>
/// ����ͼԪ���ƣ���
/// </summary>
/// <param name="flag"></param>
void BimMainWindow::on_ball_triggered(bool flag) {
	if (flag) {
		m_MyOpenGlWidget->CommandMode = CommandMode::Ball;
	}
	else {
		m_MyOpenGlWidget->CommandMode = CommandMode::None;
	}
}
