/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#include "slideglwidget.h"

SlideGLWidget::SlideGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    generalSetup();
    makeDefaultMesh();
}

SlideGLWidget::SlideGLWidget(string name, int i, QWidget *parent) :
    QGLWidget(parent)
{
    generalSetup();
    if(i == 0) {
        makeSIFMesh(name);
    } else {
        makeSLFMesh(name);
    }
    errorMsg = new QMessageBox();
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
    object2world = mat4(1);
    foreColor = QColor(255,0,0);
    backColor = QColor(0,0,0);
    whole_border = true;
    resize(600, 480);
}

void SlideGLWidget::makeDefaultMesh()
{
    makeCube(master_mesh,0.5,0.5,0.5);
    master_mesh.computeNormals();
}

void SlideGLWidget::makeSIFMesh(string name)
{
    /* Figure out the QuadSIF or SIF later.*/
    makeWithSIF(master_mesh,name);
    master_mesh.computeNormals();
    view_mesh = &master_mesh;
    temp_mesh.clear();
}

void SlideGLWidget::makeSLFMesh(string name)
{
    //Figure out the QuadSIF or SIF later/
    //makeWithSIF(master_mesh,name);
    //master_mesh.computeNormals();
    cout<<"Coming Soon!"<<endl;
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
    if(view_mesh != &master_mesh) {
        return;
    }
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
    if(selection_mode == 1){
        mySelect.selectVertex(*view_mesh, hits,buff,posX, posY, posZ);
        (hits, buff, posX, posY, posZ);
    } else if(selection_mode == 2) {
        mySelect.selectWholeBorder(*view_mesh, hits,buff,posX, posY, posZ);
    } else {
        mySelect.selectPartialBorder(*view_mesh, hits,buff,posX, posY, posZ);
    }
    glMatrixMode(GL_MODELVIEW);
    repaint();
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

    transforms[MODE_CAMERA] = lookAt(vec3(0.0,  0.0, 10.0), vec3(0.0,  0.0, 0.0), vec3(0.0,  1.0, 0.0));
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
    glClearColor(1.0f * backColor.red() / 255,
                 1.0f * backColor.green() / 255,
                 1.0f * backColor.blue() / 255,
                 1.0f * backColor.alpha() / 255);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 0, cameraDistance, 0, 0, 0, 0, 1, 0);
    glMultMatrixf(&object2world[0][0]);
    GLfloat fcolor[] = {1.0f * foreColor.red() / 255,
                        1.0f * foreColor.green() / 255,
                        1.0f * foreColor.blue() / 255,
                        1.0f * foreColor.alpha() /255};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, fcolor);
    view_mesh->drawMesh(0);
    view_mesh->drawVertices();
    if(view_mesh == &master_mesh && !temp_mesh.isEmpty())
    {
        GLfloat reversefcolor[] = {1.0f - 1.0f * foreColor.red() / 255,
                                    1.0f - 1.0f * foreColor.green() / 255,
                                    1.0f - 1.0f * foreColor.blue() / 255,
                                    1.0f - 1.0f * foreColor.alpha() /255};
        temp_mesh.drawMesh(master_mesh.vertList.size());
    }
    if(!border1.isEmpty() || !border2.isEmpty())
    {
        //float line_width = glGet(GL_LINE_WIDTH);
        glLineWidth(4.0);
        border1.drawLine();
        if(!border2.isEmpty()) {
            border2.drawLine();
        }
        glLineWidth(1.0);
    }
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
        mat3 camera2object = inverse(mat3(transforms[MODE_CAMERA]) * mat3(object2world));
        vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
        object2world = rotate(object2world, (float) ROTATION_SPEED * angle, axis_in_object_coord);
        last_mx = cur_mx;
        last_my = cur_my;
        repaint();
    }
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
        zoom_in();
        break;
    case Qt::Key_O:
        zoom_out();
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

void SlideGLWidget::subdivide(int level)
{
    if(level == 0) {
        return;
    }
    int cachedLevel = cache_subdivided_meshes.size();
    if(cachedLevel >= level) {
        subdiv_mesh = cache_subdivided_meshes[level - 1];
    } else {
        if(cachedLevel == 0) {
            subdiv_mesh = master_mesh;
        } else {
            subdiv_mesh = cache_subdivided_meshes[cachedLevel - 1];
        }
        while(cachedLevel <= level) {
            subdiv_mesh = meshCopy(subdiv_mesh);
            subdivider = new Subdivision(subdiv_mesh);
            subdiv_mesh = subdivider->ccSubdivision(1);
            subdiv_mesh.computeNormals();
            cache_subdivided_meshes.push_back(subdiv_mesh);
            cachedLevel++;
        }
        view_mesh = &subdiv_mesh;
    }
    /* Alternative.
     * subdivider = new Subdivision(m);
     * subdiv_mesh = subdivider->ccSubdivision(level);
     * subdiv_mesh.computeNormals();
     */
    repaint();
}

void SlideGLWidget::offset(float value)
{
    if(value == 0)
    {
        view_mesh = &master_mesh;
        return;
    }
    if(subdiv_mesh.isEmpty()) {
        offseter = new Offset(master_mesh, value);
    } else {
        offseter = new Offset(subdiv_mesh, value);
    }
    offseter -> makeFullOffset();
    offset_mesh = offseter->offsetMesh;
    view_mesh = &offset_mesh;
    repaint();
}

void SlideGLWidget::viewContentChanged(int view_content)
{
    switch(view_content)
    {
        case 0:
            cout<<"Not supported yet!"<<endl;
            view_mesh = &master_mesh;
            break;
        case 1:
            view_mesh = &master_mesh;
            break;
        case 2:
            view_mesh = &subdiv_mesh;
            break;
        case 3:
            view_mesh = &offset_mesh;
            break;
        case 4:
            view_mesh = &subdiv_offset_mesh;
            break;
    }
    repaint();
}

void SlideGLWidget::levelChanged(int new_level)
{
    subdivide(new_level);
    offset_mesh.clear();
    subdiv_offset_mesh.clear();
    //emit feedback_status_bar(tr("Subdivision Finished. Level: ")
    //                         + QString::number(new_level), 0);
}

void SlideGLWidget::offsetValueChanged(float new_offset_value)
{
    offset(new_offset_value);
    subdiv_offset_mesh.clear();
}

void SlideGLWidget::resetViewDirection(bool)
{
    object2world = mat4(1);
    repaint();
}

void SlideGLWidget::zoom_in()
{
    if(cameraDistance > 0.1) {
        cameraDistance *= 0.9;
    }
}

void SlideGLWidget::zoom_out()
{
    if(cameraDistance < 200) {
        cameraDistance *= 1.1;
    }
}

void SlideGLWidget::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    int numZoomIn = numDegrees.y() / 15;
    if(numZoomIn >= 1) {
        for(int i = 0; i < numZoomIn; i++) {
            zoom_in();
        }
    } else if(numZoomIn <= -1) {
        for(int i = 0; i < -numZoomIn; i++) {
            zoom_out();
        }
    }
    event->accept();
    repaint();
}

void SlideGLWidget::setForeColor(QColor color)
{
    foreColor = color;
    repaint();
}

void SlideGLWidget::setBackColor(QColor color)
{
    backColor = color;
    repaint();
}
void SlideGLWidget::addModeChecked(bool checked)
{
    if(checked)
    {
        selection_mode = 1;
    }
    else
    {
        if(whole_border)
        {
            selection_mode = 2;
        }
        else
        {
            selection_mode = 3;
        }
    }
}

void SlideGLWidget::zipModeChecked(bool checked)
{
    addModeChecked(!checked);
}

void SlideGLWidget::autoCorrectChecked(bool checked)
{
    auto_check = checked;
}

void SlideGLWidget::wholeBorderSelectionChecked(bool checked)
{
    whole_border = checked;
    if(checked) {
        selection_mode = 2;
    } else {
        selection_mode = 3;
    }
    mySelect.clearSelection();
}

void SlideGLWidget::addToTempCalled(bool)
{
    mySelect.addSelectedToMesh(temp_mesh);
}

void SlideGLWidget::zipToTempCalled(bool)
{
    if(border1.isEmpty() || border2.isEmpty()) {
        emit feedback_status_bar(tr("Please select two borders to zip!"), 0);
        return;
    }
    temp_mesh = zipper->zip(&border1, &border2, 1.3);
}

void SlideGLWidget::addTempToMaster()
{
    Mesh newMesh;
    vector<Vertex*>::iterator vIt;
    for(vIt = master_mesh.vertList.begin();
        vIt < master_mesh.vertList.end(); vIt ++)
    {
        Vertex * vertCopy = new Vertex;
        vertCopy -> ID = (*vIt) -> ID;
        vertCopy -> position = (*vIt) -> position;
        newMesh.addVertex(vertCopy);
    }
    vector<Face*>::iterator fIt;
    for(fIt = master_mesh.faceList.begin();
     fIt < master_mesh.faceList.end(); fIt ++)
    {
        Face * tempFace = *fIt;
        Edge * firstEdge = tempFace -> oneEdge;
        Edge * currEdge = firstEdge;
        Edge * nextEdge;
        vector<Vertex*> vertices;
        Vertex * tempv;
        vertices.clear();
        do {
            if(tempFace == currEdge -> fa) {
                tempv = currEdge -> vb;
                nextEdge = currEdge -> nextVbFa;
            } else {
                if(currEdge -> mobius) {
                    tempv = currEdge -> vb;
                    nextEdge = currEdge -> nextVbFb;
                } else {
                    tempv = currEdge -> va;
                    nextEdge = currEdge -> nextVaFb;
                }
            }
            vertices.push_back(newMesh.vertList[tempv -> ID]);
            currEdge = nextEdge;
        } while (currEdge != firstEdge);
        newMesh.addPolygonFace(vertices);
    }
    for(fIt = temp_mesh.faceList.begin();
     fIt < temp_mesh.faceList.end(); fIt ++)
    {
        Face * tempFace = *fIt;
        Edge * firstEdge = tempFace -> oneEdge;
        Edge * currEdge = firstEdge;
        Edge * nextEdge;
        vector<Vertex*> vertices;
        Vertex * tempv;
        vertices.clear();
        do {
            if(tempFace == currEdge -> fa) {
                tempv = currEdge -> vb;
                nextEdge = currEdge -> nextVbFa;
            } else {
                if(currEdge -> mobius) {
                    tempv = currEdge -> vb;
                    nextEdge = currEdge -> nextVbFb;
                } else {
                    tempv = currEdge -> va;
                    nextEdge = currEdge -> nextVaFb;
                }
            }
            vertices.push_back(newMesh.vertList[tempv -> ID]);
            currEdge = nextEdge;
        } while (currEdge != firstEdge);
        newMesh.addPolygonFace(vertices);
    }
    master_mesh = newMesh;
    master_mesh.buildBoundary();
    master_mesh.computeNormals();
    repaint();
}

void SlideGLWidget::addTempToMasterCalled(bool) {
    if(temp_mesh.isEmpty()) {
        emit feedback_status_bar(tr("Current temp mesh is empty."), 0);
        return;
    }
    addTempToMaster();
    clearSubDivisionAndOffset();
    clearSelectionCalled(true);
    emit feedback_status_bar(tr("Joining temp mesh into initial mesh"), 0);
}

void SlideGLWidget::addBorderCalled(bool)
{
    if(border1.isEmpty())
    {
        border1 = mySelect.addSelectedToPolyline(whole_border);
        emit feedback_status_bar(tr("First border added."), 0);
    }
    else if(border2.isEmpty())
    {
        border2 = mySelect.addSelectedToPolyline(whole_border);
        emit feedback_status_bar(tr("Second border added."), 0);
    }
    else
    {
        emit feedback_status_bar(tr("The two borders are added. You are ready to zip."), 0);
    }
}

void SlideGLWidget::clearSubDivisionAndOffset() {
    subdiv_mesh.clear();
    offset_mesh.clear();
    subdiv_offset_mesh.clear();
    cache_subdivided_meshes.clear();
}

void SlideGLWidget::clearSelection()
{
    mySelect.clearSelection();
    border1.clear();
    border2.clear();
    temp_mesh.clear();
}

void SlideGLWidget::clearSelectionCalled(bool)
{
    clearSelection();
}
