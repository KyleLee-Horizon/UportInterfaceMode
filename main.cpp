#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Moxa 协议切换工具");
    w.resize(300, 150);
    w.show();
    return a.exec();
}
