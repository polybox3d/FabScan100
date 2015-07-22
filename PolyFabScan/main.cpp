#include "PolyFabScanWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PolyFabScanWindow* w = PolyFabScanWindow::getInstance();
    w->show();

    return a.exec();
}
