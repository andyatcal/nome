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
    /* Set up the layout and widgets.*/
    void setupLayout();
    /* Build the connections. */
    void buildConnection();
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
    /* Widgets and Layout in this Control Panel.*/
    QVBoxLayout *mainLayout;
    QVBoxLayout *viewLayout;
    QVBoxLayout *modeLayout;
    QVBoxLayout *mergeLayout;
    QVBoxLayout *subdivLayout;
    QVBoxLayout *offsetLayout;
    QVBoxLayout *colorLayout;
    QComboBox *viewContent;
    QPushButton *mergeButton;
    QHBoxLayout *subdivLevelLayout;
    QSlider *subdivLevelSlider;
    QHBoxLayout *offsetValueLayout;
    QSlider *offsetValueSlider;
    QHBoxLayout *foreColorLayout;
    QHBoxLayout *backColorLayout;
    QPushButton *foreColorButton;
    QWidget *foreColorBox;
    QPushButton *backColorButton;
    QWidget *backColorBox;
};

#endif // CONTROLPANEL_H
