#ifndef DNA_H
#define DNA_H

#include <QObject>
#include <vector>

class DNA : public QObject
{
    Q_OBJECT

public:
    enum GenType {
        HP,

        defenceFire,    //defence percentage
        defenceIce,
        defenceLong,
        defenceNear,

        damageFire,     //damage points
        damageIce,
        damageLong,
        damageNear,

        dnaPoints,      //= HP + defP + damP + ap * 10
        defencePoints,  //= (defF + defI) && (defL + defN)
        damagePoints,   //= (damF + damI) && (damL + damN)
        actionpoints,

        genTypeCount
    };

    struct Gen {
        unsigned int value;
        unsigned int maxValue;
    };

public:
    DNA(QObject *parent = 0);
    DNA(const DNA &);
    ~DNA();
    unsigned getGenValue(unsigned n) const;
    unsigned getGenMaxValue(unsigned n) const;
    Gen getGen(unsigned n) const;
    bool setGenValue(unsigned value, unsigned n);
    void setRandomDNA();
    void randomMutation(unsigned f, unsigned power = 50);
    void crossover(const DNA &newdna, unsigned n = DNA::genTypeCount / 2);

    bool controlDNA() const;

    void operator=(const DNA&);

private:
    std::vector<Gen> dna;

    void correctGenValue(GenType type);
};

#endif // DNA_H
