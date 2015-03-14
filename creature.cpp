#include "creature.h"

Creature::Creature(QObject *parent) : QObject(parent)
{
    dna = new DNA();
}

Creature::~Creature()
{
    delete dna;
}

