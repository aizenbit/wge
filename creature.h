#ifndef CREATURE_H
#define CREATURE_H

#include <QObject>
#include <QPoint>

#include "dna.h"

#include "enum.h"

class Creature : public QObject
{
    Q_OBJECT

public:
    QPoint position;

public:
    Creature(CellType **m, unsigned mS, QObject *parent = 0);
    Creature(const Creature&);
    ~Creature();
    const QPoint getPosition() const;
    bool move(Direction directionX, Direction directionY);
    bool moveTo(int x, int y);
    bool attack(Creature *creature, DamageType damageType);
    void acceptDamage(int dmg, DamageType damageType);

    bool findWayTo(int x, int y);
    int stepByWay();
    void calculateDirection(QPoint myPos, QPoint targetPos,
                             int &xDir, int &yDir);

    const std::vector<QPoint> getWay() const;
    const DNA getDNA() const;
    DNA* getrDNA();
    void setAP(int);
    int getAP() const;
    int getHP() const;
    int getDamageToPlayer() const;
    void resetDamageToPlayer();
    void storeDamage(int dmg);
    bool isDead() const;
    void liven();
    void updateVariables();
    void restoreAP();

    bool operator <(const Creature &) const;
    void operator =(const Creature &);

signals:
    void paint(int delay);
    void paintAttack(QPoint a, QPoint b, DamageType dT);
    void damaged(int damage);

private:
    DNA dna;
    int HP;
    int mapSize;
    int actionpoints;
    CellType **map;
    std::vector<QPoint> way;
    bool isAlive;
    int DamageToPlayer;
};

#endif // CREATURE_H
