#include "ui.h"

#include <QPainter>

UI::UI(QWidget *parent)
    : QWidget(parent)
{
    gameMechanics = new GameMechanics();

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

    for (QRadioButton *rb : rbMap)
        connect(rb, SIGNAL(clicked()), this, SLOT(sendRBData()));


    for (int i = 0; i < 4; i++)
        hblMap[strVector[7 + i]]->insertWidget(0, rbMap[strVector[7 + i]]);

    for (int i = 0; i < DNA::genTypeCount; i++)
    {
        sbVector.push_back(new QSpinBox());
        sbVector[i]->setMaximum(gameMechanics->getPlayer().getDNA().getGenMaxValue(i));
    }

    int j = 0;
    for (int i = 0; i < 15; i++)
    {
        if(i == 1 || i == 6)            
            continue;
        hblMap[strVector[i]]->addWidget(sbVector[j]);
        j++;
    }

    setsbMapSuffix(true);
    setSBData();


    for(QSpinBox * sb : sbVector)
        sb->setMinimum(0);

    button = new QPushButton(tr("Set Data"));
    connect(button, SIGNAL(clicked()), this, SLOT(sendSBData()));

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

    for (QSpinBox *sb : sbVector)
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
        for (int i = 0; i < DNA::genTypeCount; i++)
            sbVector[i]->setSuffix(QString("/%1").arg(gameMechanics->getPlayer()
                                                 .getDNA()
                                                 .getGenMaxValue(i)));
    } else
        for (QSpinBox * sb : sbVector)
            sb->setSuffix("");
}

//------------------------------------------------------------

void UI::setSBData()
{
    const Creature p = gameMechanics->getPlayer();

    for (int i = 0; i < DNA::genTypeCount; i++)
        sbVector[i]->setValue(p.getDNA().getGenValue(i));
}

//------------------------------------------------------------

void UI::sendSBData()
{
    for (unsigned i = 0; i < DNA::genTypeCount; i++)
    {
        QPalette palette(sbVector[i]->palette());
        if (gameMechanics->getrPlayer()->getrDNA()->setGenValue(sbVector[i]->value(), i))
            palette.setColor(QPalette::Text, Qt::black);
        else
            palette.setColor(QPalette::Text, Qt::red);
        sbVector[i]->setPalette(palette);
    }

    if(!gameMechanics->getPlayer().getDNA().controlDNA())
        button->setText(QString(tr("Set Data (error!)")));
    else
        button->setText(QString(tr("Set Data")));
}

//------------------------------------------------------------

void UI::sendRBData()
{
    Damage::Type dt;

    if (rbMap[strVector[7]]->isChecked())
        dt = Damage::Ice;
    if (rbMap[strVector[8]]->isChecked())
        dt = Damage::Fire;
    if (rbMap[strVector[9]]->isChecked())
        dt = Damage::Type(dt | Damage::Long);
    if (rbMap[strVector[10]]->isChecked())
        dt = Damage::Type(dt | Damage::Near);
    gameMechanics->setPlayersDT(dt);
}
