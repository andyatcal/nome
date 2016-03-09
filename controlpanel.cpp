#include "controlpanel.h"

ControlPanel::ControlPanel()
{
    setupLayout();
}

ControlPanel::ControlPanel(SlideGLWidget * canvas)
{
    setupLayout();
    this -> canvas = canvas;
}

void ControlPanel::setupLayout() {
    QVBoxLayout *mainLayout;
    setLayout(mainLayout = new QVBoxLayout);
    QVBoxLayout *modeLayout;
    QVBoxLayout *subdivLayout;
    QVBoxLayout *offsetLayout;
    mainLayout -> addLayout(modeLayout = new QVBoxLayout);
    mainLayout -> addLayout(subdivLayout = new QVBoxLayout);
    mainLayout -> addLayout(offsetLayout = new QVBoxLayout);
    modeLayout -> addWidget(new QLabel("Mode Panel"));
    subdivLayout -> addWidget(new QLabel("Subvidision Panel"));
    offsetLayout -> addWidget(new QLabel("Offset Panel"));
    QHBoxLayout *subdivLevelLayout;
    subdivLayout -> addLayout(subdivLevelLayout = new QHBoxLayout);
    subdivLevelLayout -> addWidget(new QLabel("    Level: "));
    QSlider *subdivLevelSlider;
    subdivLevelLayout -> addWidget(subdivLevelSlider = new QSlider(Qt::Horizontal));
    subdivLevelSlider -> setMinimum(0);
    subdivLevelSlider -> setMaximum(5);
    subdivLevelSlider -> setValue(0);
    QHBoxLayout *offsetValueLayout;
    offsetLayout -> addLayout(offsetValueLayout = new QHBoxLayout);
    offsetValueLayout -> addWidget(new QLabel("    Level: "));
    QSlider *offsetValueSlider;
    offsetValueLayout -> addWidget(offsetValueSlider = new QSlider(Qt::Horizontal));
    offsetValueSlider -> setMinimum(1);
    offsetValueSlider -> setMaximum(1000);
    offsetValueSlider -> setValue(10);
}
