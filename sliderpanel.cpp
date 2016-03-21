#include "sliderpanel.h"

SliderPanel::SliderPanel()
{

}

SliderPanel::SliderPanel(ParameterBank *bank)
{
    this -> bank = bank;
    move(50, 900);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    std::cout<<"Hmm "<<(bank -> name).toStdString()<<std::endl;
    mainLayout->addWidget(new QLabel(bank -> name));
    vector<Parameter>::iterator pIt;
    for(pIt = (bank -> parameters).begin(); pIt < (bank -> parameters).end(); pIt++)
    {
        Parameter param = (*pIt);
        mainLayout -> addWidget(new MySlider(&param));
    }
}
