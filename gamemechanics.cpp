#include "gamemechanics.h"


GameMechanics::GameMechanics(QWidget *parent) : QWidget(parent)
{
    //each cell holds at least 7 pixels
    this->setMinimumSize(mapSize * 7, mapSize * 7);

    map = new CellType::CellType*[mapSize];
    for (int i = 0; i < mapSize; i++)
        map[i] = new CellType::CellType[mapSize];

    for (int i = 0; i < mapSize; i++)
        for (int j = 0; j < mapSize; j++)
            map[i][j] = CellType::empty;

    generateMap();

    //for debug
    Creature *creature1 = new Creature(map, mapSize);
    Creature *creature2 = new Creature(map, mapSize);
    delete creature1;
    delete creature2;
}

//------------------------------------------------------------

GameMechanics::~GameMechanics()
{
    for (int i = 0; i < mapSize; i++)
        delete[] map[i];
    delete [] map;
}

//------------------------------------------------------------

int GameMechanics::getMapSize() const
{
    return mapSize;
}

//------------------------------------------------------------

CellType::CellType GameMechanics::getCell(int i, int j) const
{
    return map[i][j];
}

//------------------------------------------------------------

void GameMechanics::generateMap(unsigned int seed)
{
    float ** floatMap = new float*[mapSize];
    for (int i = 0; i < mapSize; i++)
        floatMap[i] = new float[mapSize];

    srand(seed);

    //initial edges
    floatMap[0][0] = (rand() % 1000) / 1000.0f;
    floatMap[0][mapSize - 1] = (rand() % 1000) / 1000.0f;
    floatMap[mapSize - 1][0] = (rand() % 1000) / 1000.0f;
    floatMap[mapSize - 1][mapSize - 1] = (rand() % 1000) / 1000.0f;

    //generate heightmap with values between 0 and 1
    diamondSquare(floatMap);

    //assign heightmap values to cell types
    for (int i = 0; i < mapSize; i++)
        for (int j = 0; j < mapSize; j++)
        {
            CellType::CellType newCell = CellType::CellType(floatMap[i][j] * (3) + 1);

            if(newCell == CellType::empty)
                newCell = CellType::CellType(CellType::empty + 1);

            if(newCell >= CellType::busyByPlayer)
                newCell = CellType::CellType(CellType::busyByPlayer - 1);

            map[i][j] = newCell;
        }

    for (int i = 0; i < mapSize; i++)
        delete[] floatMap[i];
    delete [] floatMap;
}

//------------------------------------------------------------

void GameMechanics::diamondSquare(float ** floatMap)
{
    for (int i = 2; i < mapSize - 1; i *= 2)
    {
        square(floatMap, i);
        diamond(floatMap, i);
    }
}

//------------------------------------------------------------

void GameMechanics::diamond(float ** floatMap, int n)
{
    int side = mapSize / n;
    int halfSide = side / 2;

    //for the vertical sides of the squares
    for (int x = 0; x < mapSize - 1; x += side)
        for (int y = 0; y < mapSize - 1; y += side)
        {
            int xLeft = x - halfSide;
            int xRight = x + halfSide;
            int yCenter = y + halfSide;

            if (xLeft < 0)
                xLeft += mapSize;

            if(xRight >= mapSize)
                xRight -= mapSize;

            //random deviation is vary with squares side
            float deviation = (((rand() % 1000) / 1000.0f) - 0.5) / log2f(n);

            floatMap[x][yCenter] = (floatMap[x][y] +
                                    floatMap[x][y + side] +
                                    floatMap[xLeft][yCenter] +
                                    floatMap[xRight][yCenter]) / 4.0f +
                                    deviation;

            if(floatMap[x][yCenter] > 1)
                floatMap[x][yCenter] = 1;
            else
            if(floatMap[x][yCenter] < 0)
                floatMap[x][yCenter] = 0;
        }

    //for the horizontal sides of the squares
    for (int x = halfSide; x < mapSize - 1; x += side)
        for (int y = 0; y < mapSize - 1; y += side)
        {
            int yUpper = y - halfSide;
            int yLower = y + halfSide;

            if (yUpper < 0)
                yUpper += mapSize;

            if(yLower >= mapSize)
                yLower -= mapSize;

            //random deviation is vary with squares side
            float deviation = (((rand() % 1000) / 1000.0f) - 0.5) / log2f(n);

            floatMap[x][y] = (floatMap[x - halfSide][y] +
                              floatMap[x + halfSide][y] +
                              floatMap[x][yUpper] +
                              floatMap[x][yLower]) / 4.0f +
                              deviation;

            if(floatMap[x][y] > 1)
                floatMap[x][y] = 1;
            else
            if(floatMap[x][y] < 0)
                floatMap[x][y] = 0;
        }

}

//------------------------------------------------------------

void GameMechanics::square(float ** floatMap, int n)
{
    int side = mapSize / n;
    int halfSide = side / 2;

    for (int x = 0; x < mapSize - 1; x += side)
        for (int y = 0; y < mapSize - 1; y += side)
        {
            int xCenter = x + halfSide;
            int yCenter = y + halfSide;

            //random deviation is vary with squares side
            float deviation = (((rand() % 1000) / 1000.0f) - 0.5) / log2f(n);

            floatMap[xCenter][yCenter] = (floatMap[x][y] +
                                         floatMap[x][y + side] +
                                         floatMap[x + side][y] +
                                         floatMap[x + side][y + side]) / 4.0f +
                                         deviation;

            if(floatMap[xCenter][yCenter] > 1)
                floatMap[xCenter][yCenter] = 1;
            else
            if(floatMap[xCenter][yCenter] < 0)
                floatMap[xCenter][yCenter] = 0;
        }
}

//------------------------------------------------------------

void GameMechanics::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.begin(this);

    float cellWidth = this->width() / float(mapSize);
    float cellHeight = this->height() / float(mapSize);

    for (int i = 0; i < mapSize; i++) //paint cells according to cell types
        for (int j = 0; j < mapSize; j++)
        {
                painter.setBrush(cellColor[getCell(i, j)]);
                painter.setPen(cellColor[getCell(i, j)]);
                painter.drawRect(i * cellWidth, j * cellHeight,
                                 (i + 1) * cellWidth, (j + 1) * cellHeight);
        }

    painter.end();
}
