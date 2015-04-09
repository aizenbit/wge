#include "ui.h"

#include <QPainter>

UI::UI(QWidget *parent)
    : QWidget(parent)
{
    gameMechanics = new GameMechanics();

    stepPB = new QPushButton(tr("Next Step"));

    mainLayout = new QVBoxLayout();

    scrollArea = new QScrollArea();
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(gameMechanics);
    scrollArea->setMinimumSize(gameMechanics->getMinimumSize());

    mainLayout->addWidget(scrollArea);
    mainLayout->addWidget(stepPB);

    this->setLayout(mainLayout);
}

//------------------------------------------------------------

UI::~UI()
{
    delete gameMechanics;
    delete scrollArea;

    delete stepPB;

    delete mainLayout;
}

//------------------------------------------------------------

