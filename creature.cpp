#include <QPoint>

#include "creature.h"

Creature::Creature(CellType::CellType **m, unsigned mS, QObject *parent) : QObject(parent)
{
    //get DNA information
    HP = dna.getGenValue(DNA::HP);
    defence = dna.getGenValue(DNA::defence);
    damage = dna.getGenValue(DNA::damage);
    actionpoints = dna.getGenValue(DNA::actionpoints);

    position = QPoint(0,0);

    map = m;
    mapSize = mS;

}

//------------------------------------------------------------

Creature::~Creature()
{

}

//------------------------------------------------------------

const QPoint Creature::getPosition() const
{
    return position;
}

//------------------------------------------------------------

bool Creature::move(Direction::Direction direction)
{
    const int x = position.x();
    const int y = position.y();

    switch (direction) {

    case Direction::up:
        if (y == 0)
            return false;
        if (map[x][y - 1] == CellType::wall)
            return false;
        position.ry()--;
        break;

    case Direction::down:
        if (y == (mapSize - 1))
            return false;
        if (map[x][y + 1] == CellType::wall)
            return false;
        position.ry()++;
        break;

    case Direction::left:
        if (x == 0)
            return false;
        if (map[x - 1][y] == CellType::wall)
            return false;
        position.rx()++;

    case Direction::right:
        if (x == (mapSize - 1))
            return false;
        if (map[x + 1][y] == CellType::wall)
            return false;
        position.rx()--;

    default:
        return false;
    };

    actionpoints--;
    return true;
}

//------------------------------------------------------------

bool Creature::attack(Creature *creature)
{
    const QPoint enemyPos = creature->getPosition();
    const int damageDistance = 10;
    float distance = sqrtf(powf(enemyPos.x() - position.x(), 2) +
                           powf(enemyPos.y() - position.y(), 2));
    if (distance <= damageDistance)
    {
        int dmg = damage * logf(damageDistance - distance);
        creature->acceptDamage(dmg);

        actionpoints--;
        return true;
    }

    return false;
}

//------------------------------------------------------------

void Creature::acceptDamage(int dmg)
{
    if(dmg <= 0)
        return;

    dmg = int(float(dmg) / 100 * dmg);
    HP -= dmg;

    if (HP < 0)
        HP = 0;
}
