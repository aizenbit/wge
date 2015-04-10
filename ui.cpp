#include "ui.h"

#include <QPainter>

UI::UI(QWidget *parent)
    : QWidget(parent)
{
    gameMechanics = new GameMechanics();

    QVector <QString> strVector;

    strVector.append(tr("HP"));
    strVector.append(tr("Defence"));
    strVector.append(tr("DefenceIce"));
    strVector.append(tr("DefenceFire"));
    strVector.append(tr("DefenceLong"));
    strVector.append(tr("DefenceNear"));
    strVector.append(tr("Damage"));
    strVector.append(tr("DamageIce"));
    strVector.append(tr("DamageFire"));
    strVector.append(tr("DamageLong"));
    strVector.append(tr("DamageNear"));
    strVector.append(tr("Action Points"));
    strVector.append(tr("DNA Points"));
    strVector.append(tr("Defence Points"));
    strVector.append(tr("Damage Points"));


    for(int i = 0; i < 15; i++)
        hblMap.insert(strVector[i], new QHBoxLayout());

    for (int i = 0; i < 15; i++)
        lblMap.insert(strVector[i], new QLabel(strVector[i] + ":"));

    auto hbl = hblMap.begin();
    auto lbl = lblMap.begin();

    for (int i = 0; i < lblMap.size(); i++)
    {
        hbl.value()->addWidget(lbl.value());
        hbl++;
        lbl++;
    }

    for (int i = 0; i < 4; i++)
        rbMap.insert(strVector[7 + i], new QRadioButton());

    distanceGroup = new QButtonGroup();
    distanceGroup->addButton(rbMap[strVector[7]]);
    distanceGroup->addButton(rbMap[strVector[8]]);

    elementGroup = new QButtonGroup();
    elementGroup->addButton(rbMap[strVector[9]]);
    elementGroup->addButton(rbMap[strVector[10]]);

    for (int i = 0; i < 4; i++)
        hblMap[strVector[7 + i]]->insertWidget(0, rbMap[strVector[7 + i]]);

    int j = 0;
    for (int i = 0; i < 15; i++)
    {
        if(i == 1 || i == 6)
            continue;
        sbMap.insert(strVector[j], new QSpinBox());
        hblMap[strVector[i]]->addWidget(sbMap[strVector[j]]);
        j++;
    }

    button = new QPushButton(tr("Next Move"));

    dataLayout = new QVBoxLayout();

    for (int i = 0; i < 15; i++)
        dataLayout->addLayout(hblMap[strVector[i]]);
    dataLayout->addWidget(button);

    mainLayout = new QHBoxLayout();

    scrollArea = new QScrollArea();
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(gameMechanics);
    scrollArea->setMinimumSize(gameMechanics->getMinimumSize());

    mainLayout->addWidget(scrollArea);
    mainLayout->addSpacing(5);
    mainLayout->addLayout(dataLayout);

    this->setLayout(mainLayout);
}

//------------------------------------------------------------

UI::~UI()
{
    delete gameMechanics;
    delete scrollArea;

    delete distanceGroup;
    delete elementGroup;

    for (QRadioButton *rb : rbMap)
        delete rb;

    for (QSpinBox *sb : sbMap)
        delete sb;

    for (QLabel *lbl : lblMap)
        delete lbl;

    for (QHBoxLayout *hbl : hblMap)
        delete hbl;

    delete button;

    delete dataLayout;
    delete mainLayout;
}

//------------------------------------------------------------

