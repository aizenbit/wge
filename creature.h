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
    int DamageToPlayer;


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
    const DNA getDNA() const;
    DNA* getrDNA();
    int getAP() const;
    int getHP() const;
    int getDamageToPlayer() const;
    void resetDamageToPlayer();
    void storeDamage(int dmg);
    void liven();
    void updateVariables();
    void restoreAP();

    QPoint position;

signals:
    void paint(int delay);
    void paintAttack(QPoint a, QPoint b, Damage::Type dT);
    void damaged(int damage);

public slots:
};

#endif // CREATURE_H
