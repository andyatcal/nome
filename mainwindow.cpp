#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QGLWidget(parent)
{
    setMouseTracking(true);
    makeDefaultMesh();
    cameraDistance = 4;
}

void MainWindow::makeDefaultMesh()
{
    makeCube(mesh,0.5,0.5,0.5);
    mesh.computeNormals();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initializeGL()
{
    // Two sided pr ones side;
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_ambient0[] = { 0.8, 0.8, 0.8, 10.0 };
    GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 10.0 };
    GLfloat light_specular0[] = { 1.0, 1.0, 1.0, 10.0 };
    GLfloat light_position0[] = { 1, 1, 1, 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

    transforms[MODE_CAMERA] = lookAt(vec3(0.0,  0.0, 10.0), vec3(0.0,  0.0, 0.0), vec3(0.0,  1.0, 0.0));  // up
}

void MainWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h); // set origin to bottom left corner
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MainWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 0, cameraDistance, 0, 0, 0, 0, 1, 0);
    glMultMatrixf(&mesh.object2world[0][0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, RED);
    mesh.drawMesh();
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{

}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    printf("%d, %d\n", event->x(), event->y());
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    switch(event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    default:
        event->ignore();
        break;
    }
}
