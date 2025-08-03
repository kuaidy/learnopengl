#include "ui/BimMainWindow.h"
#include <QtWidgets/QApplication>
#include <Commands/CommandManager.h>
#include "Document.h"

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
	auto commandManager = std::make_shared<CommandManager>();
	auto document = std::make_shared<Bim::Document>();
	BimMainWindow window(nullptr, commandManager, document);
	window.show();
	return app.exec();
}
