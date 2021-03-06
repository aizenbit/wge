#include "dna.h"

DNA::DNA(QObject *parent) : QObject(parent)
{
    for(int i = 0; i < genTypeCount; i++)
        dna.push_back({0,0});

    dna[HP].maxValue = 1000;

    dna[defencePoints].maxValue = 100;
    dna[damagePoints].maxValue = 200;

    dna[damageFire].maxValue = 200;
    dna[damageIce].maxValue = 200;
    dna[damageLong].maxValue = 200;
    dna[damageNear].maxValue = 200;

    dna[defenceFire].maxValue = 100;
    dna[defenceIce].maxValue = 100;
    dna[defenceLong].maxValue = 100;
    dna[defenceNear].maxValue = 100;

    dna[actionpoints].maxValue = 9;
    dna[dnaPoints].maxValue = 1000;

    setRandomDNA();
}

//------------------------------------------------------------

DNA::DNA(const DNA &newdna) : QObject(newdna.parent())
{
    dna = newdna.dna;
}

//------------------------------------------------------------

DNA::~DNA()
{

}

//------------------------------------------------------------

void DNA::setRandomDNA()
{
    //dna[dnaPoints].value = 100 + rand() % (dna[dnaPoints].maxValue - 100);
    dna[dnaPoints].value = 1000;

    dna[HP].value = rand() % dna[dnaPoints].value;
    correctGenValue(HP);

    dna[defencePoints].value = rand() % (1 + dna[dnaPoints].value - dna[HP].value);
    correctGenValue(defencePoints);

    dna[actionpoints].value = rand() % (2 + dna[dnaPoints].value -
                                        dna[HP].value -
                                        dna[defencePoints].value);
    dna[actionpoints].value /= 10;
    correctGenValue(actionpoints);

    dna[damagePoints].value = rand() % (12 + dna[dnaPoints].value -
                                        dna[HP].value -
                                        dna[defencePoints].value -
                                        dna[actionpoints].value * 10);
    correctGenValue(damagePoints);

    dna[defenceFire].value = rand() % dna[defencePoints].value;
    dna[defenceIce].value = dna[defencePoints].value - dna[defenceFire].value;
    dna[defenceLong].value = rand() % dna[defencePoints].value;
    dna[defenceNear].value = dna[defencePoints].value - dna[defenceLong].value;

    dna[damageFire].value = rand() % dna[damagePoints].value;
    dna[damageIce].value = dna[damagePoints].value - dna[damageFire].value;
    dna[damageLong].value = rand() % dna[damagePoints].value;
    dna[damageNear].value = dna[damagePoints].value - dna[damageLong].value;

    for(int i = 0; i < genTypeCount; i++)
        correctGenValue(GenType(i));
}

//------------------------------------------------------------

void DNA::correctGenValue(GenType type)
{
    if (dna[type].value > dna[type].maxValue)
        dna[type].value = dna[type].maxValue;
    if (dna[type].value == 0)
        dna[type].value = 1;
    return;
}

//------------------------------------------------------------

bool DNA::controlDNA() const
{
    if (dna[HP].value +
            dna[defencePoints].value +
            dna[damagePoints].value +
            dna[actionpoints].value * 10 > dna[dnaPoints].value)
        return false;
    if (dna[damageFire].value + dna[damageIce].value > dna[damagePoints].value)
        return false;
    if (dna[damageLong].value + dna[damageNear].value > dna[damagePoints].value)
        return false;
    if (dna[defenceFire].value + dna[defenceIce].value > dna[defencePoints].value)
        return false;
    if (dna[defenceLong].value + dna[defenceNear].value > dna[defencePoints].value)
        return false;

    return true;
}

//------------------------------------------------------------


unsigned DNA::getGenValue(unsigned n) const
{
    return dna[n].value;
}

//------------------------------------------------------------

unsigned DNA::getGenMaxValue(unsigned n) const
{
    return dna[n].maxValue;
}

//------------------------------------------------------------

bool DNA::setGenValue(unsigned value, unsigned n)
{
    if (value > dna[n].maxValue)
    {
        value = 0;
        return false;
    }

    dna[n].value = value;

    if (controlDNA())
        return true;
    else
        return false;
}

//------------------------------------------------------------

DNA::Gen DNA::getGen(unsigned n) const
{
    return dna[n];
}

//------------------------------------------------------------

void DNA::randomMutation(unsigned f, unsigned power)
{
    if (f > genTypeCount)
        f %= genTypeCount;
    if (power == 0)
        return;

    for (Gen &gen: dna)
        if (rand() % f)
        {
            unsigned p = float(gen.maxValue) / power * 100;
            if (p == 0)
                return;
            gen.value += rand() % p;
            gen.value %= gen.maxValue;
        }
}

//------------------------------------------------------------

void DNA::crossover(const DNA &newdna, unsigned n)
{
    for (int i = n; i < genTypeCount; i++)
        dna[i].value = newdna.getGenValue(i);
}

//------------------------------------------------------------

void DNA::operator =(const DNA& newdna)
{
    for (int i = 0; i <= genTypeCount; i++)
        dna[i] = newdna.getGen(i);
}
