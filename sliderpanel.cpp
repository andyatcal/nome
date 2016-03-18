#include "sliderpanel.h"

SliderPanel::SliderPanel()
{

}

SliderPanel::SliderPanel(ParameterBank *bank)
{
    setBank(bank);
    generalSetUp();
}

void SliderPanel::setBank(ParameterBank *bank)
{
     bank = bank;
}

void SliderPanel::generalSetUp()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    vector<Parameter>::iterator pIt;
    for(pIt = (bank -> parameters).begin(); pIt < (bank -> parameters).end(); pIt++)
    {
        mainLayout -> addWidget(new MySlider(&(*pIt)));
    }
}
