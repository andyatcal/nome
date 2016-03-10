#ifndef SLIDEGLWIDGET_H
#define SLIDEGLWIDGET_H

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
#include "myselection.h"
#include "subdivison.h"
#include "stl.h"
#include <QMessageBox>

class SlideGLWidget: public QGLWidget
{
    Q_OBJECT

public:
    explicit SlideGLWidget(QWidget *parent = 0);
    /* @param name the path of the file.
     * @param i, i = 0 is the SIF file, i = 1 is the SLF file.
     */
    SlideGLWidget(string name, int i, QWidget *parent = 0);
    ~SlideGLWidget();
    // Save the current master_mesh in a STL file
    void saveMesh(string name);
    void subdivide(int level);
private:
    /* Viewer variables.*/
    enum MODES { MODE_OBJECT, MODE_CAMERA, MODE_LIGHT, MODE_LAST } view_mode;
    mat4 transforms[MODE_LAST];
    float cameraDistance;
    /* Support arcball feature.*/
    int last_mx, last_my, cur_mx, cur_my;
    /* Support arcball feature. */
    int arcball_on;
    /* object2world matrix for arcball.*/
    mat4 object2world;
    /* control of the wireframe mode. */
    bool wireframe;
    /* control of the shading mode. */
    bool smoothshading;
    /* Make a default mesh of a cube. */
    void makeDefaultMesh();
    /* Make a mesh by reading in a SIF file. */
    void makeSIFMesh(string name);
    /* Make a mesh by reading in a SLF file.*/
    void makeSLFMesh(string name);
    /* The master mesh. As a result of SIF parser.
     * Or as a result of merging the scene.
     */
    Mesh master_mesh;
    /* The working mesh that the user modified.
     * Can be merged into the master mesh.
     */
    Mesh temp_mesh;
    /* A subdivider to handler subdivision.*/
    Subdivision *subdivider;
    /* A pointer to the subdivided mesh. */
    Mesh subdiv_mesh;
    /* The cache of mesh that has been subdivided.
     * The index in this vector = subdivision level - 1.
     */
    vector<Mesh> cache_subdivided_meshes;
    /* A pointer to the offset mesh. */
    Mesh offset_mesh;
    /* A pointer to the subdivided offset mesh. */
    Mesh subdiv_offset_mesh;
    /* A pointer to the current view mesh. */
    Mesh *view_mesh;
    /* Selection object to handle mouse selection.*/
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
    /* Called by constructor to setup general background parameters. */
    void generalSetup();
    /* A message box that deliver error message.*/
    QMessageBox *errorMsg;
    /* Zoom in in the current view. */
    void zoom_in();
    /* Zoom out in the current view. */
    void zoom_out();
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
    void wheelEvent(QWheelEvent *event);
public slots:
    /* Change the current view mesh.
     * @param view_content,
     * 0 for hirachical scene
     * 1 for master mesh
     * 2 for subdivision mesh
     * 3 for offset mesh
     * 4 for subdivision on offset mesh
     */
    void viewContentChanged(int view_content);
    /* Receive the signal from control panel to do subdivision.*/
    void levelChanged(int new_level);
    /* Receive the signal to reset the viewing direction. */
    void resetViewDirection(bool checked);
    /* Receive the signal of offset value changed. */
    void offsetValueChanged(float value);
signals:
};

#endif // SLIDEGLWIDGET_H
