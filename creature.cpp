#include <QPoint>

#include "creature.h"

Creature::Creature(CellType::CellType **m, unsigned mS, QObject *parent) : QObject(parent)
{
    //get DNA information
    HP = dna.getGenValue(DNA::HP);
    actionpoints = dna.getGenValue(DNA::actionpoints);
    actionpoints = 50;
    position = QPoint(0,0);

    map = m;
    mapSize = mS;

}

//------------------------------------------------------------

Creature::Creature(const Creature &creature) : QObject(creature.parent())
{
    dna = creature.dna;
    HP = creature.HP;
    actionpoints = creature.actionpoints;

    position = creature.position;

    map = creature.map;
    mapSize = creature.mapSize;

    way = creature.way;
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

const std::vector<QPoint> Creature::getWay() const
{
    return way;
}

//------------------------------------------------------------

bool Creature::move(Direction::Direction directionX, Direction::Direction directionY)
{
    const int x = position.x();
    const int y = position.y();

    if (x + directionX < 0 || x + directionX >= mapSize)
        return false;

    if (y + directionY < 0 || y + directionY >= mapSize)
        return false;

    if (map[x + directionX][y + directionY] == CellType::wall)
        return false;

    position.rx() += directionX;
    position.ry() += directionY;
    actionpoints--;

    return true;
}

//------------------------------------------------------------

bool Creature::moveTo(int x, int y)
{
    if (!findWayTo(x,y))
        return false;

    if(way.size() > actionpoints)
        return false;

    while (!way.empty() && actionpoints > 0)
    {
        position.rx() = way.back().x();
        position.ry() = way.back().y();
        way.erase(way.end());
        actionpoints--;
        emit paint(50);
    }

    return true;
}

//------------------------------------------------------------

bool Creature::attack(Creature *creature, Damage::Type damageType)
{
    const QPoint enemyPos = creature->getPosition();
    const int nearDamageDistance = 10;
    const int longDamageDistance = 25;

    float distance = sqrtf(powf(enemyPos.x() - position.x(), 2) +
                           powf(enemyPos.y() - position.y(), 2));

    if (damageType & Damage::Near)
    {
        if (distance >= nearDamageDistance)
            return false;

        int dmg = dna.getGenValue(DNA::damageNear) * logf(nearDamageDistance - distance);

        if(damageType & Damage::Fire)
            dmg += dna.getGenValue(DNA::damageFire);

        if(damageType & Damage::Ice)
            dmg += dna.getGenValue(DNA::damageIce);

        creature->acceptDamage(dmg, damageType);
        actionpoints--;
        return true;
    }

    if (damageType & Damage::Long)
    {
        if (distance >= longDamageDistance)
            return false;

        int dmg = dna.getGenValue(DNA::damageLong);// * logf(abs(distance - longDamageDistance));

        if(damageType & Damage::Fire)
            dmg += dna.getGenValue(DNA::damageFire);

        if(damageType & Damage::Ice)
            dmg += dna.getGenValue(DNA::damageIce);

        creature->acceptDamage(dmg, damageType);
        actionpoints--;
        return true;
    }

    return false;
}

//------------------------------------------------------------

void Creature::acceptDamage(int dmg, Damage::Type damageType)
{
    if (dmg <= 0)
        return;

    if((damageType & Damage::Long) &&
       (damageType & Damage::Near))
        return;

    if (damageType & Damage::Fire)
        dmg = dmg * dna.getGenValue(DNA::defenceFire) / 100;

    if (damageType & Damage::Ice)
        dmg = dmg * dna.getGenValue(DNA::defenceIce) / 100;

    if (damageType & Damage::Long)
        dmg = dmg * dna.getGenValue(DNA::defenceLong) / 100;

    if (damageType & Damage::Near)
        dmg = dmg * dna.getGenValue(DNA::defenceNear) / 100;

    HP -= dmg;

    if (HP < 0)
        HP = 0;
}

//------------------------------------------------------------

bool Creature::findWayTo(int x, int y)
{
    if (map[x][y] == CellType::wall)
        return false;

    //find way using Lee algorithm
    //Initialisation

    int myX = position.x();
    int myY = position.y();

    int ** labelMap = new int*[mapSize];
    for (int i = 0; i < mapSize; i++)
        labelMap[i] = new int[mapSize];

    for (int i = 0; i < mapSize; i++)
        for (int j = 0; j < mapSize; j++)
            labelMap[i][j] = -2;
    labelMap[myX][myY] = 0;

    int step = 0;
    bool canMove = true;

    //Wave expansion
    while (labelMap[x][y] == -2 && canMove)
    {
        canMove = false;
        for (int i = 0; i < mapSize; i++)
            for (int j = 0; j < mapSize; j++)
                if (labelMap[i][j] == step)

                    for (int k = -1; k <= 1; k++)
                        for (int l = -1; l <= 1; l++)
                        {
                            int tx = i + k; //temporary x and y
                            int ty = j + l;

                            if (tx >= mapSize || tx < 0 ||
                                ty >= mapSize || ty < 0)
                                continue;

                            if (labelMap[tx][ty] == -2)
                            {
                                if (map[tx][ty] == CellType::wall)
                                    labelMap[tx][ty] = -1;
                                    else
                                {
                                    labelMap[tx][ty] = step + 1;
                                    canMove = true;
                                }
                            }
                        }
        step++;
    }

    //Backtrace
    way.clear();
    way.push_back(QPoint(x, y));

    while(step > 0)
    {
        int tx = way.back().x();
        int ty = way.back().y();
        bool found = false;
        bool optimalDirection = false;

        //calculate direction vector
        int xDirection = myX - tx;
        int yDirection = myY - ty;
        calculateDirection(QPoint(way.back()), position, xDirection, yDirection);

        if(abs(xDirection) > abs(yDirection))
        {
            yDirection = 0;
            xDirection /= abs(xDirection);
        }
        else
            if (abs(xDirection) < abs(yDirection))
            {
                xDirection = 0;
                yDirection /= abs(yDirection);
            }
            else
            {
                xDirection /= abs(xDirection);
                yDirection /= abs(yDirection);
            }

        //search next cell,
        //cell in the right direction is prioritized
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
            {
                if (optimalDirection ||
                    tx + i >= mapSize || tx + i < 0 ||
                    ty + j >= mapSize || ty + j < 0)
                    continue;

                if (labelMap[tx + i][ty + j] < labelMap[tx][ty] &&
                    labelMap[tx + i][ty + j] >= 0)
                {
                    if (xDirection == i && yDirection == j)
                    {
                        if (found)
                            way.back() = QPoint(tx + i, ty + j);
                        else
                            way.push_back(QPoint(tx + i, ty + j));

                        optimalDirection = true;
                        found = true;
                    }

                    if (!found)
                    {
                        way.push_back(QPoint(tx + i, ty + j));
                        found = true;
                    }

                }
            }
        step--;
    }

    //Clearance
    for (int i = 0; i < mapSize; i++)
        delete[] labelMap[i];
    delete [] labelMap;

    return true;
}

//------------------------------------------------------------

void Creature::calculateDirection(QPoint myPos, QPoint targetPos,
                                   int & xDir,
                                   int & yDir)
{
    int xDirection = targetPos.x() - myPos.x();
    int yDirection = targetPos.y() - myPos.y();

    if(abs(xDirection) > abs(yDirection))
    {
        yDirection = 0;
        xDirection /= abs(xDirection);
    }
    else
        if (abs(xDirection) < abs(yDirection))
        {
            xDirection = 0;
            yDirection /= abs(yDirection);
        }
        else
        {
            xDirection /= abs(xDirection);
            yDirection /= abs(yDirection);
        }
    xDir = xDirection;
    yDir = yDirection;
}

//------------------------------------------------------------

int Creature::stepByWay()
{
    if (way.empty())
        return -3;

    if (abs(way.front().x() - position.x()) > 1 ||
        abs(way.front().y() - position.y()) > 1)
        return -2;

    if (way.front().x() == position.x() &&
        way.front().y() == position.y())
        return -1;

    position.rx() = way.front().x();
    position.ry() = way.front().y();

    way.erase(way.begin());

    return 0;
}
