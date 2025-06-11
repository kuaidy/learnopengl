#include "Bim.h"

Bim::Bim(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	myOpenGlWidget = new MyOpenGLWidget(parent);
	setCentralWidget(myOpenGlWidget);
}

Bim::~Bim()
{
}

void Bim::OpenModule(string path) {
	
}

void Bim::on_btnopenfile_clicked() {
	qDebug() << "按钮点击事件触发！";
}
