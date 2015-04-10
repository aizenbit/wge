#include "ui.h"

#include <QPainter>

UI::UI(QWidget *parent)
    : QWidget(parent)
{
    gameMechanics = new GameMechanics();

    QVector <QString> strVector;

    strVector.append(tr("HP"));
    strVector.append(tr("Defence"));
    strVector.append(tr("Ice Defence"));
    strVector.append(tr("Fire Defence"));
    strVector.append(tr("Long Defence"));
    strVector.append(tr("Near Defence"));
    strVector.append(tr("Damage"));
    strVector.append(tr("Ice Damage"));
    strVector.append(tr("Fire Damage"));
    strVector.append(tr("Long Damage"));
    strVector.append(tr("Near Damage"));
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

    int j = -1;
    for (int i = 0; i < 15; i++)
    {
        j++;
        if(i == 1 || i == 6)            
            continue;
        sbMap.insert(strVector[j], new QSpinBox());
        hblMap[strVector[i]]->addWidget(sbMap[strVector[j]]);
    }

    sbMap["HP"]->setMaximum(1000);
    sbMap["Ice Defence"]->setMaximum(100);
    sbMap["Fire Defence"]->setMaximum(100);
    sbMap["Long Defence"]->setMaximum(100);
    sbMap["Near Defence"]->setMaximum(100);
    sbMap["Ice Damage"]->setMaximum(100);
    sbMap["Fire Damage"]->setMaximum(100);
    sbMap["Long Damage"]->setMaximum(100);
    sbMap["Near Damage"]->setMaximum(100);
    sbMap["Action Points"]->setMaximum(9);
    sbMap["DNA Points"]->setMaximum(200);
    sbMap["Defence Points"]->setMaximum(100);
    sbMap["Damage Points"]->setMaximum(100);

    setsbMapSuffix(true);

    for(QSpinBox * sb : sbMap)
        sb->setMinimum(0);

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

void UI::setsbMapSuffix(bool set)
{
    if (set)
    {
        sbMap["HP"]->setSuffix("/1000");
        sbMap["Ice Defence"]->setSuffix(QString("/100"));
        sbMap["Fire Defence"]->setSuffix(QString("/100"));
        sbMap["Long Defence"]->setSuffix(QString("/100"));
        sbMap["Near Defence"]->setSuffix(QString("/100"));
        sbMap["Ice Damage"]->setSuffix(QString("/100"));
        sbMap["Fire Damage"]->setSuffix(QString("/100"));
        sbMap["Long Damage"]->setSuffix(QString("/100"));
        sbMap["Near Damage"]->setSuffix(QString("/100"));
        sbMap["Action Points"]->setSuffix(QString("/9"));
        sbMap["DNA Points"]->setSuffix(QString("/200"));
        sbMap["Defence Points"]->setSuffix(QString("/100"));
        sbMap["Damage Points"]->setSuffix(QString("/100"));
    } else
        for (QSpinBox * sb : sbMap)
            sb->setSuffix("");
}
