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
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#endif
#include "mesh.h"
#include "makeMesh.h"

class MainWindow : public QGLWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    // Viewer variables.
    Mesh mesh;
    enum MODES { MODE_OBJECT, MODE_CAMERA, MODE_LIGHT, MODE_LAST } view_mode;
    mat4 transforms[MODE_LAST];
    float cameraDistance;
    void makeDefaultMesh();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
};

#endif // MAINWINDOW_H
