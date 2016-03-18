#include "myslider.h"

MySlider::MySlider()
{
}

MySlider::MySlider(Parameter *param)
{
    setParameter(param);
    generalSetup();
}

void MySlider::generalSetup()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(new QLabel(param -> name));
    QHBoxLayout *sliderLayout;
    mainLayout -> addLayout(sliderLayout = new QHBoxLayout);
    sliderLayout -> addWidget(new QLabel(QString::number(param -> start)));
    QSlider *slider;
    sliderLayout -> addWidget(slider = new QSlider());
    slider -> setMinimum(param -> start);
    slider -> setMaximum(param -> end);
    slider -> setValue(param -> value);
    sliderLayout -> addWidget(new QLabel(QString::number(param -> end)));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(changeValue(int)));
}

void MySlider::setParameter(Parameter *param)
{
    this -> param = param;
}

void MySlider::changeValue(int newValue)
{
    param -> value = newValue;
}

