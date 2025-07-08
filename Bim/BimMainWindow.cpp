#include "BimMainWindow.h"

BimMainWindow::BimMainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_MyOpenGlWidget = new MyOpenGLWidget(ui.frameopengl);
	// 设置布局
	QVBoxLayout* layout = new QVBoxLayout(ui.frameopengl);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_MyOpenGlWidget);
	// 可选：设置大小策略
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
		tr("选择模型文件"),
		"",
		tr("模型文件(*.gltf *.glb *.obj *.fbx *.stl *.ifc);;所有文件 (*.*)")
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
/// 绘制b样条曲线
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
/// 基本图元绘制，球
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
