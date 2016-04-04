#include "sliderpanel.h"

SliderPanel::SliderPanel()
{

}

SliderPanel::SliderPanel(ParameterBank *bank, SlideGLWidget * canvas)
{
    this -> bank = bank;
    move(50, 900);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->addWidget(new QLabel(bank -> name));
    vector<Parameter*>::iterator pIt;
    for(pIt = (bank -> parameters).begin(); pIt < (bank -> parameters).end(); pIt++)
    {
        Parameter *param = (*pIt);
        MySlider *newSlider = new MySlider(param);
        param -> slider = newSlider;
        mainLayout -> addWidget(newSlider);
        connect(newSlider, SIGNAL(paramValueChanged(float)), canvas, SLOT(paramValueChanged(float)));
    }
}
