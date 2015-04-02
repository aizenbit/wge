#include <QPoint>

#include "creature.h"

Creature::Creature(CellType::CellType **m, unsigned mS, QObject *parent) : QObject(parent)
{
    //get DNA information
    HP = dna.getGenValue(DNA::HP);
    defence = dna.getGenValue(DNA::defence);
    damage = dna.getGenValue(DNA::damage);
    actionpoints = dna.getGenValue(DNA::actionpoints);

    position = QPoint(2,2);

    map = m;
    mapSize = mS;

}

//------------------------------------------------------------

Creature::Creature(const Creature &creature) : QObject(creature.parent())
{
    dna = creature.dna;
    HP = creature.HP;
    defence = creature.defence;
    damage = creature.damage;
    actionpoints = creature.actionpoints;

    position = creature.position;

    map = creature.map;
    mapSize = creature.mapSize;
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
                {
                    for (int k = -1; k <= 1; k++)
                        for (int l = -1; l <= 1; l++)
                        {
                            int tx = i + k;
                            int ty = j + l;
                            if (tx > mapSize || tx < 0 ||
                                ty > mapSize || ty < 0)
                                continue;
                            if ( labelMap[tx][ty] == -2 )
                            {
                                if ( map[tx][ty] == CellType::wall )
                                    labelMap[tx][ty] = -1;
                                    else
                                {
                                    labelMap[tx][ty] = step + 1;
                                    canMove = true;
                                }
                            }
                        }
                }
    }

    if (labelMap[x][y] == -2)
        return false;

    //Backtrace
    way.clear();
    step = labelMap[x][y];
    way.push_back(QPoint(x,y));

    while(step >= 0)
    {
        int tx = way.back().x();
        int ty = way.back().y();
        bool found = false;

        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
            {
                if (!found || tx + i > mapSize || tx + i < 0 ||
                              ty + j > mapSize || ty + j < 0)
                    continue;

                if(labelMap[x + i][y + j] < labelMap[x][y] &&
                   labelMap[x + i][y + j] >= 0)
                {
                    way.push_back(QPoint(x + i, y + j));
                    step++;
                    found = true;
                }
            }
    }

    //Clearance
    for (int i = 0; i < mapSize; i++)
        delete[] labelMap[i];
    delete [] labelMap;

    return true;
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
