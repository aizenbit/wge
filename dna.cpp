#include "dna.h"

DNA::DNA(QObject *parent) : QObject(parent)
{
    for(int i = 0; i < genTypeCount; i++)
        dna.push_back({0,0});

    dna[HP].maxValue = 1000;

    dna[defencePoints].maxValue = 100;
    dna[damagePoints].maxValue = 500;

    dna[damageFire].maxValue = 100;
    dna[damageIce].maxValue = 100;
    dna[damageLong].maxValue = 100;
    dna[damageNear].maxValue = 100;

    dna[defenceFire].maxValue = 100;
    dna[defenceIce].maxValue = 100;
    dna[defenceLong].maxValue = 100;
    dna[defenceNear].maxValue = 100;

    dna[actionpoints].maxValue = 9;
    dna[dnaPoints].maxValue = 200;

    for (Gen &gen: dna)
        do
            gen.value = rand() % gen.maxValue;
        while (!controlDNA());

}

//------------------------------------------------------------

DNA::~DNA()
{

}

//------------------------------------------------------------

bool DNA::controlDNA()
{
    if (dna[HP].value + dna[defencePoints].value + dna[damagePoints].value > dna[dnaPoints].value)
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

DNA::Gen DNA::getGen(unsigned n) const
{
    return dna[n];
}

//------------------------------------------------------------

void DNA::randomMutation(unsigned f, unsigned power)
{
    f %= genTypeCount;
    f = genTypeCount - f;

    for (Gen &gen: dna)
        if (rand() % f)
        {
            unsigned p = float(gen.maxValue) / power * 100;
            gen.value += rand() % p;
            gen.value %= gen.maxValue;
        }
}

//------------------------------------------------------------

void DNA::operator =(const DNA& newdna)
{
    for (int i = 0; i <= genTypeCount; i++)
    {
        dna[i] = newdna.getGen(i);
    }
}
