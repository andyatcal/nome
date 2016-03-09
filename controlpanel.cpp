#include "controlpanel.h"

ControlPanel::ControlPanel()
{
    setupLayout();
}

ControlPanel::ControlPanel(SlideGLWidget * canvas)
{
    setupLayout();
    this -> canvas = canvas;
    buildConnection();
}

void ControlPanel::buildConnection()
{
    /* Build our connections. */
    connect(viewContent, SIGNAL(currentIndexChanged(int)), canvas, SLOT(changeViewContent(int)));
}

void ControlPanel::setupLayout()
{
    /* Main Layout.
     * Contains view, mode, merge, subdivision, offset, color.*/
    setLayout(mainLayout = new QVBoxLayout);
    mainLayout -> addLayout(viewLayout = new QVBoxLayout);
    mainLayout -> addLayout(modeLayout = new QVBoxLayout);
    mainLayout -> addLayout(mergeLayout = new QVBoxLayout);
    mainLayout -> addLayout(subdivLayout = new QVBoxLayout);
    mainLayout -> addLayout(offsetLayout = new QVBoxLayout);
    mainLayout -> addLayout(colorLayout = new QVBoxLayout);
    /* View layout. */
    viewLayout -> addWidget(new QLabel("View Panel"));
    viewLayout -> addWidget(new QLabel("View Content:"));
    viewLayout -> addWidget(viewContent = new QComboBox);
    viewContent -> addItem("Initial Mesh");
    viewContent -> addItem("Subdivision Mesh");
    viewContent -> addItem("Offset Mesh");
    viewContent -> addItem("Subdivision on Offset Mesh");
    /* Mode layout.*/
    modeLayout -> addWidget(new QLabel("Mode Panel"));
    /* Merge layout. */
    mergeLayout -> addWidget(new QLabel("Merge Panel"));
    mergeLayout -> addWidget(mergeButton = new QPushButton(tr("Merge")));
    /* Subdivision layout. */
    subdivLayout -> addWidget(new QLabel("Subvidision Panel"));
    subdivLayout -> addLayout(subdivLevelLayout = new QHBoxLayout);
    subdivLevelLayout -> addWidget(new QLabel("    Level: "));
    subdivLevelLayout -> addWidget(subdivLevelSlider = new QSlider(Qt::Horizontal));
    subdivLevelSlider -> setMinimum(0);
    subdivLevelSlider -> setMaximum(5);
    subdivLevelSlider -> setValue(0);
    /* Offset Layout. */
    offsetLayout -> addWidget(new QLabel("Offset Panel"));
    offsetLayout -> addLayout(offsetValueLayout = new QHBoxLayout);
    offsetValueLayout -> addWidget(new QLabel("    Value: "));
    offsetValueLayout -> addWidget(offsetValueSlider = new QSlider(Qt::Horizontal));
    offsetValueSlider -> setMinimum(1);
    offsetValueSlider -> setMaximum(1000);
    offsetValueSlider -> setValue(10);
    /* Color Layout. */
    colorLayout -> addWidget(new QLabel("Color Panel"));
    colorLayout -> addLayout(foreColorLayout = new QHBoxLayout);
    colorLayout -> addLayout(backColorLayout = new QHBoxLayout);
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
