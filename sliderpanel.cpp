#include "sliderpanel.h"

SliderPanel::SliderPanel()
{

}

void SliderPanel::setParameters(vector<Parameter*>);

void generalSetUp()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    vector<Parameter*>::iterator pIt;
    for(pIt = parameters.begin(); pIt < parameters.end(); pIt++)
    {
        mainLayout -> addWidget(new MySlider(*pIt));
    }
}
