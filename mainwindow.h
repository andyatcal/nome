#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtOpenGL>
#define ROTATION_SPEED (1.0)
#if __linux__
 #include <GL/glut.h>
 #include <GL/gl.h>
 #define ROTATION_SPEED (50.0)
#elif __APPLE__
 #include <GLUT/GLUT.h>
 #define ROTATION_SPEED (1.0)
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
#include "polyline.h"
class MainWindow : public QGLWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    // Viewer variables.
    enum MODES { MODE_OBJECT, MODE_CAMERA, MODE_LIGHT, MODE_LAST } view_mode;
    mat4 transforms[MODE_LAST];
    float cameraDistance;
    int last_mx, last_my, cur_mx, cur_my;
    int arcball_on;
    bool wireframe;
    bool smoothshading;
    void makeDefaultMesh();
    Mesh master_mesh;
    Mesh temp_mesh;
    /**
     * Get a normalized vector from the center of the virtual ball O to a
     * point P on the virtual ball surface, such that P is aligned on
     * screen's (X,Y) coordinates.  If (X,Y) is too far away from the
     * sphere, return the nearest point on the virtual ball surface.
     */
    vec3 get_arcball_vector(int x, int y);
    /**
     * Timer Event for this MainWindow. Similar to OnIdleFunc of GLUT.
     */
    void timerEvent(QTimerEvent *event);
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
};

#endif // MAINWINDOW_H
