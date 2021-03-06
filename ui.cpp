#include "ui.h"

#include <QPainter>

UI::UI(QWidget *parent)
    : QWidget(parent)
{
    gameMechanics = new GameMechanics();

    gameMechanics->getrPlayer()->setAP(0);

    connect(gameMechanics, SIGNAL(newWave()), this, SLOT(newWave()));

    for(int i = 0; i < DNA::genTypeCount; i++)
        hblVector.push_back(new QHBoxLayout());

    for (int i = 0; i < DNA::genTypeCount; i++)
        lblVector.push_back(new QLabel(gameMechanics->strVector[i] + ":"));

    for (int i = 0; i < DNA::genTypeCount; i++)
        hblVector[i]->addWidget(lblVector[i]);

    for (int i = DNA::damageFire; i <= DNA::damageNear; i++)
        rbVector.push_back(new QRadioButton());

    distanceGroup = new QButtonGroup();
    distanceGroup->addButton(rbVector[2]);
    distanceGroup->addButton(rbVector[3]);

    elementGroup = new QButtonGroup();
    elementGroup->addButton(rbVector[0]);
    elementGroup->addButton(rbVector[1]);

    for (QRadioButton *rb : rbVector)
        connect(rb, SIGNAL(clicked()), this, SLOT(sendRBData()));

    for (int i = DNA::damageFire; i <= DNA::damageNear; i++)
        hblVector[i]->insertWidget(0, rbVector[i - DNA::damageFire]);

    for (int i = 0; i < DNA::genTypeCount; i++)
    {
        sbVector.push_back(new QSpinBox());
        sbVector[i]->setMaximum(gameMechanics->getPlayer().getDNA().getGenMaxValue(i));
    }

    for (int i = 0; i < DNA::genTypeCount; i++)
        hblVector[i]->addWidget(sbVector[i]);

    setsbMapSuffix(true);
    setSBData();

    for(QSpinBox * sb : sbVector)
        sb->setMinimum(0);

    button = new QPushButton(tr("Set Data"));
    connect(button, SIGNAL(clicked()), this, SLOT(sendSBData()));
    connect(gameMechanics, SIGNAL(playersMove(bool)),
            button, SLOT(setEnabled(bool)));

    damageGB = new QGroupBox(tr("Damage"));
    defenceGB = new QGroupBox(tr("Defence"));
    damageLayout = new QVBoxLayout();
    defenceLayout = new QVBoxLayout();

    for (int i = DNA::damageFire; i <= DNA::damageNear; i++)
        damageLayout->addLayout(hblVector[i]);
    for (int i = DNA::defenceFire; i <= DNA::defenceNear; i++)
        defenceLayout->addLayout(hblVector[i]);

    damageGB->setLayout(damageLayout);
    defenceGB->setLayout(defenceLayout);

    dataLayout = new QVBoxLayout();

    dataLayout->addLayout(hblVector[DNA::HP]);
    dataLayout->addWidget(defenceGB);
    dataLayout->addWidget(damageGB);
    for (int i = DNA::damageNear + 1; i < DNA::genTypeCount; i++)
        dataLayout->addLayout(hblVector[i]);
    dataLayout->addWidget(button);

    mainLayout = new QHBoxLayout();

    scrollArea = new QScrollArea();
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(gameMechanics);
    scrollArea->setMinimumSize(gameMechanics->getSizeForSA());

    mainLayout->addWidget(scrollArea);
    mainLayout->addSpacing(5);
    mainLayout->addLayout(dataLayout);

    this->setLayout(mainLayout);
    this->setWindowTitle(tr("Worst Game Ever"));
}

//------------------------------------------------------------

UI::~UI()
{
    delete gameMechanics;
    delete scrollArea;

    delete distanceGroup;
    delete elementGroup;

    for (QRadioButton *rb : rbVector)
        delete rb;

    for (QSpinBox *sb : sbVector)
        delete sb;

    for (QLabel *lbl : lblVector)
        delete lbl;

    for (QHBoxLayout *hbl : hblVector)
        delete hbl;

    delete defenceGB;
    delete damageGB;

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

    gameMechanics->getrPlayer()->updateVariables();
    gameMechanics->repaint();

    if (!gameMechanics->getPlayer().getDNA().controlDNA())
    {
        button->setText(QString(tr("Set Data (error!)")));
        gameMechanics->getrPlayer()->setAP(0);
    }
    else
    {
        for (QSpinBox *sb : sbVector)
        {
            sb->setReadOnly(true);
            QPalette palette(sb->palette());
            palette.setColor(QPalette::Text, Qt::black);
            sb->setPalette(palette);
        }

        button->setText(QString(tr("Next Move")));
        disconnect(button, SIGNAL(clicked()), this, SLOT(sendSBData()));
        connect(button, SIGNAL(clicked()), gameMechanics, SLOT(nextMove()));
    }
}

//------------------------------------------------------------

void UI::sendRBData()
{
    DamageType dt = DamageType::None;

    if (rbVector[0]->isChecked())
        dt = DamageType::Fire;
    if (rbVector[1]->isChecked())
        dt = DamageType::Ice;
    if (rbVector[2]->isChecked())
        dt = DamageType(dt | DamageType::Long);
    if (rbVector[3]->isChecked())
        dt = DamageType(dt | DamageType::Near);

    gameMechanics->setPlayersDT(dt);
}

//------------------------------------------------------------

void UI::newWave()
{
    button->setText(QString(tr("Set Data")));
    for (QSpinBox *sb : sbVector)
        sb->setReadOnly(false);
    disconnect(button, SIGNAL(clicked()), gameMechanics, SLOT(nextMove()));
    connect(button, SIGNAL(clicked()), this, SLOT(sendSBData()));
    gameMechanics->getrPlayer()->liven();
}
