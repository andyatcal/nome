#include "slideglwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SlideGLWidget s;
    s.show();
    return a.exec();
}
