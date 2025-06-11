#include "Bim.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Bim window;
    window.show();
    return app.exec();
}
