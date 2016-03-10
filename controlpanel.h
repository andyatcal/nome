#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <iostream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>
#include <QColorDialog>
#include <QLineEdit>
#include <QStatusBar>
#include <QCheckBox>
#include "slideglwidget.h"
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
    /* Widgets and Layout in this Control Panel.*/
    QVBoxLayout *mainLayout;
    QVBoxLayout *viewLayout;
    QGroupBox *modeGroupBox;
    QVBoxLayout *modeLayout;
    QHBoxLayout *modeSelectLayout;
    QHBoxLayout *zipLayout;
    QHBoxLayout *addLayout;
    QRadioButton *addModeButton;
    QRadioButton *zipModeButton;
    QPushButton *addButton;
    QPushButton *zipButton;
    QCheckBox *autoCorrectCheck;
    QCheckBox *wholeBorderCheck;
    QVBoxLayout *subdivLayout;
    QVBoxLayout *offsetLayout;
    QVBoxLayout *colorLayout;
    QComboBox *viewContent;
    QPushButton *resetViewButton;
    QPushButton *mergeButton;
    QHBoxLayout *subdivLevelLayout;
    QLabel *currentLevelLabel;
    QSlider *subdivLevelSlider;
    QHBoxLayout *offsetMinMaxLayout;
    QHBoxLayout *offsetValueLayout;
    QSlider *offsetValueSlider;
    QLabel *currentOffsetValueLabel;
    QHBoxLayout *foreColorLayout;
    QHBoxLayout *backColorLayout;
    QPushButton *foreColorButton;
    QWidget *foreColorBox;
    QPushButton *backColorButton;
    QWidget *backColorBox;
    float minOffset;
    float maxOffset;
    int offsetStep;
    QLineEdit *minOffsetBox;
    QLineEdit *maxOffsetBox;
    QLineEdit *offsetStepBox;
    QStatusBar *statusBar;
public slots:
    void test(QString test);
    void test(bool);
    void viewContentReset();
    void viewContentSetToSubdiv(int level);
    void viewContentSetToSubdivOffset();
    void resetMinOffset(QString minOffset);
    void resetMaxOffset(QString maxOffset);
    void resetOffsetStep(QString offsetStep);
    void offSetSliderMoved();
    void resetForeColor(bool);
    void resetBackColor(bool);
signals:
    void makeOffsetMesh(float);
};


#endif // CONTROLPANEL_H
