#include <QPoint>

#include "creature.h"

Creature::Creature(QObject *parent) : QObject(parent)
{
    dna = new DNA();
    HP = dna->getGenValue(DNA::HP);
    defence = dna->getGenValue(DNA::defence);

    position = new QPoint(0,0);
}

//------------------------------------------------------------

Creature::~Creature()
{
    delete dna;

    delete position;
}
