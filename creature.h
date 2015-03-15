#ifndef CREATURE_H
#define CREATURE_H

#include <QObject>
#include <QPoint>

#include "dna.h"

#include "enum.h"

class Creature : public QObject
{
    Q_OBJECT

private:
    DNA dna;
    int HP;
    int defence;
    int damage;
    int mapSize;
    int actionpoints;
    CellType::CellType **map;


public:
    explicit Creature(CellType::CellType **m, unsigned mS, QObject *parent = 0);
    ~Creature();
    const QPoint getPosition() const;
    bool move(Direction::Direction direction);
    bool attack(Creature *creature);
    void acceptDamage(int dmg);

    QPoint position;

signals:

public slots:
};

#endif // CREATURE_H
