#include "Bim.h"

Bim::Bim(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_MyOpenGlWidget = new MyOpenGLWidget(parent);
	setCentralWidget(m_MyOpenGlWidget);
}

Bim::~Bim()
{
}

void Bim::OpenModule(string path) {

}

void Bim::on_fileopen_triggered() {
	qDebug() << "按钮点击事件触发！";
}
void Bim::on_modelline_triggered(bool flag) {
	if (flag) {
		m_MyOpenGlWidget->CommandMode = CommandMode::ModelLine;
		m_MyOpenGlWidget->CreateModelLine();
	}
	else {
		m_MyOpenGlWidget->CommandMode = CommandMode::None;
	}
}
