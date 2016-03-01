#ifndef SLIDEGLWIDGET_H
#define SLIDEGLWIDGET_H

#include <QtOpenGL>
#define ROTATION_SPEED (1.0)
#if __linux__
 #include <GL/glut.h>
 #include <GL/gl.h>
 #define ROTATION_SPEED (50.0)
#elif __APPLE__
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
#include "myselection.h"
class SlideGLWidget: public QOpenGLWidget
{
    Q_OBJECT

public:
    SlideGLWidget(QWidget *parent = 0);
    ~SlideGLWidget();

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
    MySelection mySelect;
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
    // A wrapper function for selection with mouse
    // @param x, the x coordinate of the mouse clicked
    // @param y, the y coordinate of the mouse clicked
    // @param mode, the mode of selection.
    void mouse_select(int x, int y);
    // selection_mode = 1: vertex selection
    // selection_mode = 2: whole border selection (line loop)
    // selection_mode = 3: partial border selection (line strip)
    int selection_mode;
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
};

#endif // SLIDEGLWIDGET_H
