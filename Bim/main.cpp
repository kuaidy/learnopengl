#include "BimMainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

//#ifdef Q_OS_WIN
//	QFont font("Microsoft YaHei");
//#elif defined(Q_OS_MAC)
//	QFont font("PingFang SC");
//#else
//	QFont font("WenQuanYi Micro Hei");
//#endif
//	qApp->setFont(font);

	BimMainWindow window;
	window.show();
	return app.exec();
}
