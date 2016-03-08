#include "mainwindow.h"

MainWindow::MainWindow()
{
    createActions();
    createMenus();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Import Input File"), "/", tr("Geometry Files (*.slf *.sif)"));
    if(fileName == "") {
        return;
    }
    createCanvas(fileName);
    createControlPanel();
}

void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this,
         tr("Save STL File"), "/", tr("STL Files (*.stl)"));
    if(fileName == "") {
        return;
    } else if(canvas == NULL) {
        cout<<"Error: No work to be saved!"<<endl;
        return;
    }
    canvas -> saveMesh(fileName.toStdString());
}

void MainWindow::close()
{
    if(canvas != NULL) {
        canvas -> close();
        canvas = NULL;
        controls -> close();
        controls = NULL;
    }
}

void MainWindow::quit()
{
    exit(1);
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a SIF or SLF file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save your work as a STL file"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    closeAct = new QAction(tr("&Close..."), this);
    closeAct->setShortcuts(QKeySequence::Close);
    closeAct->setStatusTip(tr("Close the file and canvas"));
    connect(closeAct, SIGNAL(triggered()), this, SLOT(close()));

    quitAct = new QAction(tr("&Quit..."), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Open an existing file"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(quit()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(closeAct);
    fileMenu->addAction(quitAct);
}
void MainWindow::createCanvas(QString name)
{
    canvas = new SlideGLWidget(name.toStdString());
    canvas -> move(0, 50);
    canvas -> show();
}

void MainWindow::createControlPanel()
{
    controls = new ControlPanel();
    controls -> move(900, 50);
    controls -> show();
}
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.exec(event->globalPos());
}
