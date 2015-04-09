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
    int mapSize;
    int actionpoints;
    CellType::CellType **map;
    std::vector<QPoint> way;


public:
    Creature(CellType::CellType **m, unsigned mS, QObject *parent = 0);
    Creature(const Creature&);
    ~Creature();
    const QPoint getPosition() const;
    bool move(Direction::Direction directionX, Direction::Direction directionY);
    bool attack(Creature *creature, DamageType::DamageType damageType);
    void acceptDamage(int dmg, DamageType::DamageType damageType);
    bool findWayTo(int x, int y);
    int stepByWay();
    const std::vector<QPoint> getWay() const;

    QPoint position;

signals:

public slots:
};

#endif // CREATURE_H
