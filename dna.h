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

        defenceFire,
        defenceIce,
        defenceLong,
        defenceNear,

        damageFire,
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

private:
    std::vector<Gen> dna;

    void correctGenValue(GenType type);

public:
    DNA(QObject *parent = 0);
    DNA(const DNA &);
    ~DNA();
    unsigned getGenValue(unsigned n) const;
    unsigned getGenMaxValue(unsigned n) const;
    Gen getGen(unsigned n) const;
    void randomMutation(unsigned f, unsigned power = 50);
    void setRandomDNA();
    bool setGenValue(unsigned value, unsigned n);
    bool controlDNA() const;

    void operator=(const DNA&);

signals:

public slots:
};

#endif // DNA_H
