#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtOpenGL>
#if __linux__
 #include <GL/glut.h>
 #include <GL/gl.h>
#elif __APPLE__
 #include <GLUT/GLUT.h>
#endif

class MainWindow : public QGLWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
};

#endif // MAINWINDOW_H
