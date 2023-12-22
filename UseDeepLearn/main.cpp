#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    DealWithImage::getStatic()->Init();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
