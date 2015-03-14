#ifndef CREATURE_H
#define CREATURE_H

#include <QObject>
#include <QPoint>

#include "dna.h"

class Creature : public QObject
{
    Q_OBJECT

private:
    DNA dna;
    unsigned int HP;
    unsigned int defence;
    QPoint position;

public:
    explicit Creature(QObject *parent = 0);
    ~Creature();

signals:

public slots:
};

#endif // CREATURE_H
