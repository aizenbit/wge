#include "gamemechanics.h"

GameMechanics::GameMechanics(QObject *parent) : QObject(parent)
{
    map = new CellType*[mapSize];
    for (int i = 0; i < mapSize; i++)
        map[i] = new CellType[mapSize];

    for (int i = 0; i < mapSize; i++)
        for (int j = 0; j < mapSize; j++)
            map[i][j] = empty;
}

GameMechanics::~GameMechanics()
{
    for (int i = 0; i < mapSize; i++)
        delete[] map[i];
    delete [] map;
}

int GameMechanics::getMapSize() const
{
    return mapSize;
}

CellType GameMechanics::getCell(int i, int j) const
{
    return map[i][j];
}
