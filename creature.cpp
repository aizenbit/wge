#include <QPoint>

#include "creature.h"

Creature::Creature(QObject *parent) : QObject(parent)
{
    HP = dna.getGenValue(DNA::HP);
    defence = dna.getGenValue(DNA::defence);

    position = QPoint(0,0);
}

//------------------------------------------------------------

Creature::~Creature()
{

}
