#include "mainwindow.h"
#include "slideglwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //SlideGLWidget s;
    //s.show();
    MainWindow window;
    window.show();
    return a.exec();
}
