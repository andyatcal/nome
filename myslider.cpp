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
    setLayout(mainLayout);
    mainLayout -> addWidget(new QLabel((param -> name) + QString(" : ") +QString::number(param -> value)));
    QHBoxLayout *sliderLayout;
    mainLayout -> addLayout(sliderLayout = new QHBoxLayout);
    sliderLayout -> addWidget(new QLabel(QString::number(param -> start)));
    QSlider *slider;
    sliderLayout -> addWidget(slider = new QSlider(Qt::Horizontal));
    slider -> setMinimum(0);
    slider -> setMaximum(int((param -> end - param -> start) / param -> stepsize));
    slider -> setValue(int((param -> value - param -> start) / param -> stepsize));
    sliderLayout -> addWidget(new QLabel(QString::number(param -> end)));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(changeValue(int)));
}

void MySlider::setParameter(Parameter *param)
{
    this -> param = param;
}

void MySlider::changeValue(int newValue)
{
    param -> value = newValue * (param -> stepsize) + param -> start;
}

