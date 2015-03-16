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
        defence,
        damage,
        actionpoints,
        genTypeCount
    };

    struct Gen {
        unsigned int value;
        unsigned int maxValue;
    };

private:
    std::vector<Gen> dna;


public:
    DNA(QObject *parent = 0);
    ~DNA();
    unsigned getGenValue(unsigned n) const;
    unsigned getGenMaxValue(unsigned n) const;
    Gen getGen(unsigned n) const;
    void randomMutation(unsigned f, unsigned power = 50);

    void operator=(const DNA&);

signals:

public slots:
};

#endif // DNA_H
