#ifndef SLIDERPANEL_H
#define SLIDERPANEL_H

#include <QWidget>
#include <iostream>
#include <QVBoxLayout>
#include "myslider.h"
#include "parameter.h"
#include "parameterbank.h"
#include "slideglwidget.h"
class SliderPanel : public QWidget
{
    Q_OBJECT

public:
    SliderPanel();
    SliderPanel(ParameterBank *bank, SlideGLWidget * canvas);
    ParameterBank *bank;
};

#endif // SLIDERPANEL_H
