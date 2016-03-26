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
    MySlider(Parameter *param);
    void generalSetup();
    void setParameter(Parameter *param);
private:
    Parameter *param;
    QLabel *currValue;
public slots:
    void changeValue(int);
signals:
    void paramValueChanged(float);
};

#endif // MYSLIDER_H
