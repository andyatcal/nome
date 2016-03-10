/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

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
    void createCanvas(QString name);
    void createControlPanel(SlideGLWidget *canvas);
    QMenu *fileMenu;
    QAction *openAct;
    QAction *saveAct;
    QAction *closeAct;
    QAction *quitAct;
    SlideGLWidget *canvas;
    ControlPanel *controls;
private slots:
    void open();
    void save();
    void close();
    void quit();
protected:
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
};

#endif // MAINWINDOW_H
