#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "slideglwidget.h"
#include "controlpanel.h"

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
private:
    void createActions();
    void createMenus();
    void createCanvas();
    QMenu *fileMenu;
    QAction *openAct;
    SlideGLWidget *canvas;
    ControlPanel *controls;
private slots:
    void open();
protected:
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
};

#endif // MAINWINDOW_H
