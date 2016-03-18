#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QWidget>
#include "parameter.h"
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class MySlider : public QWidget
{
    Q_OBJECT

public:
    MySlider();
    void setParameter(Parameter *param);
private:
    Parameter *param;
public slots:
    void changeValue(int);
};

#endif // MYSLIDER_H
