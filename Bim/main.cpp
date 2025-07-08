#include "BimMainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    BimMainWindow window;
    window.show();
    return app.exec();
}
