#include "slideglwidget.h"

SlideGLWidget::SlideGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    generalSetup();
    makeDefaultMesh();
}

SlideGLWidget::SlideGLWidget(string name, QWidget *parent) :
    QGLWidget(parent)
{
    generalSetup();
    makeMesh(name);
}

void SlideGLWidget::generalSetup()
{
    startTimer(0);
    cameraDistance = 4;
    last_mx = last_my = cur_mx = cur_my = 0;
    arcball_on = false;
    wireframe = false;
    smoothshading = true;
    selection_mode = 0;
}

void SlideGLWidget::makeDefaultMesh()
{
    makeCube(master_mesh,0.5,0.5,0.5);
    master_mesh.computeNormals();
}

void SlideGLWidget::makeMesh(string name)
{
    // Figure out the QuadSIF or SIF later/
    makeWithSIF(master_mesh,name);
    master_mesh.computeNormals();
}

void SlideGLWidget::saveMesh(string name)
{
    // Figure out the QuadSIF or SIF later/
    STL *stl = new STL;
    stl -> STLOutput(master_mesh, name);
    master_mesh.computeNormals();
}

vec3 SlideGLWidget::get_arcball_vector(int x, int y) {
    vec3 p = vec3(1.0 * x / this->width() * 2 - 1.0,
      1.0 * y / this->height() * 2 - 1.0, 0);
    p.y = - p.y;
    float op_squared = p.x * p.x + p.y * p.y;
    if (op_squared <= 1 * 1) {
        p.z = sqrt(1 * 1 - op_squared);
    } else {
        p = normalize(p);  // nearest point
    }
    return p;
}

void SlideGLWidget::mouse_select(int x, int y) {
    makeCurrent();
    GLuint buff[64] = {0};
    GLint hits, view[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
    glSelectBuffer(64, buff);
    glGetIntegerv(GL_VIEWPORT, view);
    // Find the 3D points of the current clicked point
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev(GL_PROJECTION_MATRIX, projection );
    winX = (double) x;
    winY = (double) view[3] - (double)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    cout<<"winX "<<winX<<" "<<"winY "<<winY<<" "<<"winZ "<<winZ<<endl;
    gluUnProject( winX, winY, winZ, modelview, projection,
     view, &posX, &posY, &posZ);
    //cout<<"X: "<<posX<<" Y: "<<posY<<" Z: "<<posZ<<endl;
    // Find the face selected.
    glRenderMode(GL_SELECT);
    //glClearColor(0, 0, 0, 1);
    glInitNames();
    glPushName(0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    //commit
    gluPickMatrix(x, view[3] - y, 1.0, 1.0, view);
    gluPerspective(45, (float) this -> width() / this -> height(), 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
    repaint();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    hits = glRenderMode(GL_RENDER);
    cout<<posX<<" "<<posY<<" "<<posZ<<endl;
    //mySelect.list_hits(hits, buff);
    if(selection_mode == 0){
        mySelect.selectVertex(master_mesh, hits,buff,posX, posY, posZ);
        (hits, buff, posX, posY, posZ);
    } else if(selection_mode == 1) {
        mySelect.selectWholeBorder(master_mesh, hits,buff,posX, posY, posZ);
    } else {
        mySelect.selectPartialBorder(master_mesh, hits,buff,posX, posY, posZ);
    }
    glMatrixMode(GL_MODELVIEW);
}
SlideGLWidget::~SlideGLWidget()
{

}

void SlideGLWidget::initializeGL()
{
    //Smooth Shading
    glShadeModel(GL_SMOOTH);
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

void SlideGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float) w / h, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SlideGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 0, cameraDistance, 0, 0, 0, 0, 1, 0);
    glMultMatrixf(&master_mesh.object2world[0][0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, RED);
    master_mesh.drawMesh();
    master_mesh.drawVertices();
}

void SlideGLWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        arcball_on = true;
        last_mx = cur_mx = event -> x();
        last_my = cur_my = event -> y();
    }
    else
    {
        arcball_on = false;
    }
    if (event->buttons() & Qt::RightButton)
    {
        mouse_select(event -> x(), event -> y());
    }
}

void SlideGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    if(arcball_on)
    {
      cur_mx = event -> x();
      cur_my = event -> y();
    }
}

void SlideGLWidget::timerEvent(QTimerEvent *event) {
    if(last_mx != cur_mx || last_my != cur_my) {
        vec3 va = get_arcball_vector(last_mx, last_my);
        vec3 vb = get_arcball_vector( cur_mx,  cur_my);
        float angle = acos(glm::min(1.0f, dot(va, vb)));
        vec3 axis_in_camera_coord = cross(va, vb);
        mat3 camera2object = inverse(mat3(transforms[MODE_CAMERA]) * mat3(master_mesh.object2world));
        vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
        master_mesh.object2world = rotate(master_mesh.object2world, (float) ROTATION_SPEED * angle, axis_in_object_coord);
        last_mx = cur_mx;
        last_my = cur_my;
    }
    repaint();
}
void SlideGLWidget::keyPressEvent(QKeyEvent* event)
{
    switch(event->key()) {
    case Qt::Key_Escape:
        mySelect.clearSelection();
        break;
    case Qt::Key_W:
        if (wireframe) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        } else {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        }
        wireframe = !wireframe;
        break;
    case Qt::Key_I:
        if(cameraDistance > 0.1) {
            cameraDistance *= 0.9;
        }
        break;
    case Qt::Key_O:
        if(cameraDistance < 200) {
            cameraDistance *= 1.1;
        }
        break;
    case Qt::Key_S:
        if (smoothshading) {
            glShadeModel(GL_FLAT);
        } else {
            glShadeModel(GL_SMOOTH);
        }
        smoothshading = !smoothshading;
        break;
    default:
        event->ignore();
        break;
    }
    repaint();
}

