#ifndef CREATURE_H
#define CREATURE_H

#include <QObject>
#include <QPoint>

#include "dna.h"

#include "celltype.h"

class Creature : public QObject
{
    Q_OBJECT

private:
    DNA dna;
    unsigned int HP;
    unsigned int defence;
    unsigned int mapSize;
    CellType **map;


public:
    explicit Creature(CellType **m, unsigned mS, QObject *parent = 0);
    ~Creature();

    QPoint position;

signals:

public slots:
};

#endif // CREATURE_H
