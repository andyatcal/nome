#ifndef SLIDERPANEL_H
#define SLIDERPANEL_H

#include <QWidget>
#include <iostream>
#include <QVBoxLayout>
#include "myslider.h"
#include "parameter.h"
class SliderPanel : public QWidget
{
    Q_OBJECT

public:
    SliderPanel();
    void setParameters(vector<Parameter*>);
    vector<Parameter*> parameters;
};

#endif // SLIDERPANEL_H
