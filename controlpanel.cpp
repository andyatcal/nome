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
    /* Main Layout.
     * Contains view, mode, subdivision, offset, color.*/
    QVBoxLayout *mainLayout;
    setLayout(mainLayout = new QVBoxLayout);
    QVBoxLayout *viewLayout;
    QVBoxLayout *modeLayout;
    QVBoxLayout *subdivLayout;
    QVBoxLayout *offsetLayout;
    QVBoxLayout *colorLayout;
    mainLayout -> addLayout(viewLayout = new QVBoxLayout);
    mainLayout -> addLayout(modeLayout = new QVBoxLayout);
    mainLayout -> addLayout(subdivLayout = new QVBoxLayout);
    mainLayout -> addLayout(offsetLayout = new QVBoxLayout);
    mainLayout -> addLayout(colorLayout = new QVBoxLayout);
    /* View layout. */
    viewLayout -> addWidget(new QLabel("View Panel"));
    QHBoxLayout *viewContentLayout;
    viewLayout -> addWidget(new QLabel("View Content:"));
    QComboBox *viewContent;
    viewLayout -> addWidget(viewContent = new QComboBox);
    viewContent -> addItem("Initial Mesh");
    viewContent -> addItem("Subdivision Mesh");
    viewContent -> addItem("Offset Mesh");
    viewContent -> addItem("Subdivision on Offset Mesh");
    /* Mode layout.*/
    modeLayout -> addWidget(new QLabel("Mode Panel"));
    /* Subdivision layout. */
    subdivLayout -> addWidget(new QLabel("Subvidision Panel"));
    QHBoxLayout *subdivLevelLayout;
    subdivLayout -> addLayout(subdivLevelLayout = new QHBoxLayout);
    subdivLevelLayout -> addWidget(new QLabel("    Level: "));
    QSlider *subdivLevelSlider;
    subdivLevelLayout -> addWidget(subdivLevelSlider = new QSlider(Qt::Horizontal));
    subdivLevelSlider -> setMinimum(0);
    subdivLevelSlider -> setMaximum(5);
    subdivLevelSlider -> setValue(0);
    /* Offset Layout. */
    offsetLayout -> addWidget(new QLabel("Offset Panel"));
    QHBoxLayout *offsetValueLayout;
    offsetLayout -> addLayout(offsetValueLayout = new QHBoxLayout);
    offsetValueLayout -> addWidget(new QLabel("    Value: "));
    QSlider *offsetValueSlider;
    offsetValueLayout -> addWidget(offsetValueSlider = new QSlider(Qt::Horizontal));
    offsetValueSlider -> setMinimum(1);
    offsetValueSlider -> setMaximum(1000);
    offsetValueSlider -> setValue(10);
    /* Color Layout. */
    colorLayout -> addWidget(new QLabel("Color Panel"));
    QHBoxLayout *foreColorLayout;
    QHBoxLayout *backColorLayout;
    colorLayout -> addLayout(foreColorLayout = new QHBoxLayout);
    colorLayout -> addLayout(backColorLayout = new QHBoxLayout);
    QPushButton *foreColorButton;
    QWidget *foreColorBox;
    QPushButton *backColorButton;
    QWidget *backColorBox;
    foreColorLayout -> addWidget(foreColorButton = new QPushButton(tr("Foreground Color")));
    foreColorLayout -> addWidget(foreColorBox = new QWidget());
    foreColorBox -> resize(5,5);
    QPalette forePal = foreColorBox->palette();
    forePal.setColor(foreColorBox->backgroundRole(), QColor(255,0,0));
    foreColorBox->setPalette(forePal);
    foreColorBox -> setAutoFillBackground(true);
    backColorLayout -> addWidget(backColorButton = new QPushButton(tr("Background Color")));
    backColorLayout -> addWidget(backColorBox = new QWidget());
    QPalette backPal = foreColorBox->palette();
    backPal.setColor(backColorBox->backgroundRole(), QColor(0,0,0));
    backColorBox->setPalette(backPal);
    backColorBox -> setAutoFillBackground(true);
    backColorBox -> resize(5,5);
}
