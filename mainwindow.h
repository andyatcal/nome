/**
 * @author Andy Wang, UC Berkeley.
 * Copyright 2016 reserve.
 * UC Berkeley, Slide_2016 project.
 * Advised by Prof. Sequin H. Carlos.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include "slideglwidget.h"
#include "controlpanel.h"
#include "nomeparser.h"
#include "sliderpanel.h"
#include <sstream>
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
    void createSliderPanel(SlideGLWidget * canvas);
    void save_current_status_nome(string out_put_file);
    void save_current_status_anom(string out_put_file);
    QMenu *fileMenu;
    QAction *openAct;
    QAction *saveAct;
    QAction *closeAct;
    QAction *quitAct;
    SlideGLWidget *canvas;
    ControlPanel *controls;
    NomeParser *nomeParser;
    Group scene;
    Group append_scene;
    unordered_map<string, Parameter> params;
    vector<ParameterBank> banks;
    vector<SliderPanel*> slider_panels;
    vector<string> colorlines;
    vector<string> banklines;
    vector<string> geometrylines;
    vector<string> postProcessingLines;
private slots:
    void open();
    void save();
    void close();
    void quit();
protected:
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
};

#endif // MAINWINDOW_H
