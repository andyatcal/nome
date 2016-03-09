#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

using namespace std;

#include <QWidget>
#include <iostream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>
#include <QColorDialog>
#include "slideglwidget.h"
namespace Ui {
class ControlPanel;
}
class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    ControlPanel();
    /* @param canvas, the canvas that this panel control.*/
    ControlPanel(SlideGLWidget * canvas);
    void setupLayout();
    /* The selection mode of current program.
     * 1: vertex selection.
     * 2: whole border secletion.
     * 3: partial border selection.
     */
    int selectionMode;
    /* The subdivision level.*/
    int subdivLevel;
    /* Offset value. */
    float offsetValue;
private:
    SlideGLWidget *canvas;
    Ui::ControlPanel *ui;
};

#endif // CONTROLPANEL_H
