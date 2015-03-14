#include "dna.h"

DNA::DNA(QObject *parent) : QObject(parent)
{
    for(int i = 0; i < genTypeCount; i++)
        dna.push_back({0,0});

    dna[0].maxValue = 1000;
    dna[1].maxValue = 100;

    for (Gen &gen: dna)
    {
        gen.value = rand() % gen.maxValue;
    }
}

//------------------------------------------------------------

DNA::~DNA()
{

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

DNA::Gen DNA::getGene(unsigned n) const
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
        dna[i] = newdna.getGene(i);
    }
}
