#include <QPoint>

#include "creature.h"

Creature::Creature(CellType **m, unsigned mS, QObject *parent) : QObject(parent)
{
    HP = dna.getGenValue(DNA::HP);
    defence = dna.getGenValue(DNA::defence);

    position = QPoint(0,0);

    map = m;
    mapSize = mS;
}

//------------------------------------------------------------

Creature::~Creature()
{

}
