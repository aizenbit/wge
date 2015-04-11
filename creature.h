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
    bool isAlive;


public:
    Creature(CellType::CellType **m, unsigned mS, QObject *parent = 0);
    Creature(const Creature&);
    ~Creature();
    const QPoint getPosition() const;
    bool move(Direction::Direction directionX, Direction::Direction directionY);
    bool moveTo(int x, int y);
    bool attack(Creature *creature, Damage::Type damageType);
    void acceptDamage(int dmg, Damage::Type damageType);
    bool findWayTo(int x, int y);
    void calculateDirection(QPoint myPos, QPoint targetPos,
                             int &xDir, int &yDir);
    int stepByWay();
    const std::vector<QPoint> getWay() const;
    bool isDead() const;

    QPoint position;

signals:
    void paint(int delay);
    void paintAttack(QPoint a, QPoint b, Damage::Type dT);

public slots:
};

#endif // CREATURE_H
