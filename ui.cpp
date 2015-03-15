#include "ui.h"

#include <QPainter>

UI::UI(QWidget *parent)
    : QWidget(parent)
{
    gameMechanics = new GameMechanics();

    stepPB = new QPushButton(tr("Next Step"));

    mainLayout = new QVBoxLayout();

    mainLayout->addWidget(gameMechanics);
    mainLayout->addWidget(stepPB);

    this->setLayout(mainLayout);
}

//------------------------------------------------------------

UI::~UI()
{
    delete gameMechanics;

    delete stepPB;

    delete mainLayout;
}

//------------------------------------------------------------

