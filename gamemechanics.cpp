#include "gamemechanics.h"


GameMechanics::GameMechanics(QWidget *parent) : QWidget(parent)
{
    //each cell holds at least 10 pixels
    this->setMinimumSize(mapSize * 10, mapSize * 10);

    map = new CellType::CellType*[mapSize];
    for (int i = 0; i < mapSize; i++)
        map[i] = new CellType::CellType[mapSize];

    for (int i = 0; i < mapSize; i++)
        for (int j = 0; j < mapSize; j++)
            map[i][j] = CellType::empty;

    generateMap();

    curCellPos = QPoint(0,0);

    player = new Creature(map, mapSize);

    //for debug
    player->move(Direction::right, Direction::down);
    player->move(Direction::right, Direction::down);

    for(int i = 0; i < mapSize-20; i++)
        map[i][20] = CellType::wall;

    Creature creature1(map, mapSize);
    //Creature creature2(map, mapSize);
    creature1.findWayTo(50,60);
    enemy.push_back(creature1);
    //enemy.push_back(creature2);
    player->attack(&creature1, Damage::Type(Damage::Near | Damage::Fire));
}

//------------------------------------------------------------

GameMechanics::~GameMechanics()
{
    delete player;

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

QSize GameMechanics::getMinimumSize() const
{
    QSize size = minimumSize();
    size.rheight() += 2;
    size.rwidth() += 2;
    return size;
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

    paintMap(painter);
    paintEnemy(painter);
    //paintWay(painter, enemy.front());
    paintPlayer(painter);
    paintCelectedCell(painter);

    painter.end();
}

//------------------------------------------------------------

void GameMechanics::paintMap(QPainter &painter)
{
    float cellWidth = this->width() / float(mapSize);
    float cellHeight = this->height() / float(mapSize);

    for (int i = 0; i < mapSize; i++) //paint cells according to cell types
        for (int j = 0; j < mapSize; j++)
        {
                painter.setBrush(cellColor[getCell(i, j)]);
                painter.setPen(cellColor[getCell(i, j)]);
                painter.drawRect(i * cellWidth, j * cellHeight,
                                 cellWidth, cellHeight);
        }
}

//------------------------------------------------------------

void GameMechanics::paintPlayer(QPainter &painter)
{
    float cellWidth = this->width() / float(mapSize);
    float cellHeight = this->height() / float(mapSize);

    painter.setBrush(cellColor[CellType::busyByPlayer]);
    painter.setPen(cellColor[CellType::busyByPlayer]);

    painter.drawRect(player->getPosition().x() * cellWidth,
                     player->getPosition().y() * cellHeight,
                     cellWidth,
                     cellHeight);
}

//------------------------------------------------------------

void GameMechanics::paintEnemy(QPainter &painter)
{
    float cellWidth = this->width() / float(mapSize);
    float cellHeight = this->height() / float(mapSize);

    painter.setBrush(cellColor[CellType::busyByEnemy]);
    painter.setPen(cellColor[CellType::busyByEnemy]);

    for (Creature creature : enemy)
    {
        int xb = creature.getPosition().x() * cellWidth;
        int yb = creature.getPosition().y() * cellWidth;
        painter.drawRect(xb, yb, cellWidth, cellHeight);
    }
}

//------------------------------------------------------------

void GameMechanics::paintWay(QPainter &painter, const Creature &creature)
{
    float cellWidth = this->width() / float(mapSize);
    float cellHeight = this->height() / float(mapSize);

    painter.setBrush(Qt::yellow);
    painter.setPen(Qt::yellow);

    for (QPoint point : creature.getWay())
        painter.drawRect(point.x() * cellWidth,
                         point.y() * cellHeight,
                         cellWidth,
                         cellHeight);
}

//------------------------------------------------------------

void GameMechanics::paintCelectedCell(QPainter &painter)
{
    float cellWidth = this->width() / float(mapSize);
    float cellHeight = this->height() / float(mapSize);

    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::white);

    painter.drawRect(curCellPos.x() * cellWidth,
                     curCellPos.y() * cellHeight,
                     cellWidth,
                     cellHeight);
}

//------------------------------------------------------------

void GameMechanics::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        showToolTip(event);
    if (event->button() == Qt::LeftButton)
        selectCell(event);
}

//------------------------------------------------------------

void GameMechanics::showToolTip(QMouseEvent *event)
{
    QPoint pos = event->pos();
    QString text;

    int i = pos.x() / (this->width() / float(mapSize));
    int j = pos.y() / (this->height() / float(mapSize));

    text = QString(tr("(%1;%2)\n"
                      "CellType = %3")).arg(i).arg(j)
                                       .arg(int(map[i][j]));

    QToolTip::showText(this->mapToGlobal(pos), text, this);
}

//------------------------------------------------------------

void GameMechanics::selectCell(QMouseEvent *event)
{
    QPoint pos = event->pos();
    int i = pos.x() / (this->width() / float(mapSize));
    int j = pos.y() / (this->height() / float(mapSize));

    curCellPos = QPoint(i,j);
    repaint();
}
