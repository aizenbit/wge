#include "gamemechanics.h"
#include <algorithm>

GameMechanics::GameMechanics(QWidget *parent) : QWidget(parent)
{
    this->setMinimumSize(mapSize * cellSize, mapSize * cellSize);

    map = new CellType::CellType*[mapSize];
    for (int i = 0; i < mapSize; i++)
        map[i] = new CellType::CellType[mapSize];

    for (int i = 0; i < mapSize; i++)
        for (int j = 0; j < mapSize; j++)
            map[i][j] = CellType::empty;

    generateMap();

    curCellPos = QPoint(0,0);
    attack = false;
    allEnemiesDead = false;

    player = new Creature(map, mapSize);
    for (int i = 0; i < DNA::genTypeCount; i++)
        player->getrDNA()->setGenValue(0, i);

    player->getrDNA()->setGenValue(1000, DNA::dnaPoints);

    //for debug

    player->getrDNA()->setGenValue(100, DNA::defencePoints);
    player->getrDNA()->setGenValue(99, DNA::defenceFire);
    player->getrDNA()->setGenValue(99, DNA::defenceLong);
    player->getrDNA()->setGenValue(200, DNA::damagePoints);
    player->getrDNA()->setGenValue(200, DNA::damageFire);
    player->getrDNA()->setGenValue(200, DNA::damageLong);
    player->getrDNA()->setGenValue(9, DNA::actionpoints);
    player->getrDNA()->setGenValue(100, DNA::HP);

    for (int i = 0; i <= 5; i++)
        enemy.push_back(Creature(map, mapSize));


    for (Creature &creature : enemy)
    {
        connect(&creature, SIGNAL(paint(int)),
                this, SLOT(paint(int)));
        connect(&creature, SIGNAL(paintAttack(QPoint, QPoint, Damage::Type)),
                this, SLOT(paintAttack(QPoint, QPoint, Damage::Type)));
    }

    connect(player, SIGNAL(paint(int)),
            this, SLOT(paint(int)));
    connect(player, SIGNAL(paintAttack(QPoint, QPoint, Damage::Type)),
            this, SLOT(paintAttack(QPoint, QPoint, Damage::Type)));
    connect(player, SIGNAL(damaged(int)),
            this, SLOT(storeDamage(int)));

    strVector.append(tr("HP"));
    strVector.append(tr("Fire"));   //Defence
    strVector.append(tr("Ice"));    //Defence
    strVector.append(tr("Long"));   //Defence
    strVector.append(tr("Near"));   //Defence
    strVector.append(tr("Fire"));   //Damage
    strVector.append(tr("Ice"));    //Damage
    strVector.append(tr("Long"));   //Damage
    strVector.append(tr("Near"));   //Damage
    strVector.append(tr("DNA Points"));
    strVector.append(tr("Defence Points"));
    strVector.append(tr("Damage Points"));
    strVector.append(tr("Action Points"));
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

const Creature GameMechanics::getPlayer() const
{
    return *player;
}

//------------------------------------------------------------

Creature* GameMechanics::getrPlayer()
{
    return player;
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
    //paintWay(painter, *player);
    paintPlayer(painter);
    paintCelectedCell(painter);
    paintDead(painter);
    if (attack)
        paintAttack(painter);

    painter.end();
}

//------------------------------------------------------------

void GameMechanics::paintMap(QPainter &painter)
{
    for (int i = 0; i < mapSize; i++) //paint cells according to cell types
        for (int j = 0; j < mapSize; j++)
        {
                painter.setBrush(cellColor[getCell(i, j)]);
                painter.setPen(cellColor[getCell(i, j)]);
                painter.drawRect(i * cellSize, j * cellSize,
                                 cellSize, cellSize);
        }
    this->setMinimumSize(cellSize * mapSize, cellSize * mapSize);
}

//------------------------------------------------------------

void GameMechanics::paintPlayer(QPainter &painter)
{
    painter.setBrush(cellColor[CellType::busyByPlayer]);
    painter.setPen(cellColor[CellType::busyByPlayer]);

    painter.drawRect(player->getPosition().x() * cellSize,
                     player->getPosition().y() * cellSize,
                     cellSize,
                     cellSize);
}

//------------------------------------------------------------

void GameMechanics::paintEnemy(QPainter &painter)
{
    painter.setBrush(cellColor[CellType::busyByEnemy]);
    painter.setPen(cellColor[CellType::busyByEnemy]);

    for (Creature creature : enemy)
    {
        int xb = creature.getPosition().x() * cellSize;
        int yb = creature.getPosition().y() * cellSize;
        painter.drawRect(xb, yb, cellSize, cellSize);
    }
}

//------------------------------------------------------------

void GameMechanics::paintWay(QPainter &painter, const Creature &creature)
{
    painter.setBrush(Qt::yellow);
    painter.setPen(Qt::yellow);

    for (QPoint point : creature.getWay())
        painter.drawRect(point.x() * cellSize,
                         point.y() * cellSize,
                         cellSize,
                         cellSize);
}

//------------------------------------------------------------

void GameMechanics::paintCelectedCell(QPainter &painter)
{
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::white);

    painter.drawRect(curCellPos.x() * cellSize,
                     curCellPos.y() * cellSize,
                     cellSize,
                     cellSize);
}

//------------------------------------------------------------

void GameMechanics::paintAttack(QPainter &painter)
{
    painter.setPen(Qt::gray);

    if (damageType & Damage::Fire)
        painter.setPen(Qt::red);
    if (damageType & Damage::Ice)
        painter.setPen(Qt::blue);

    painter.drawLine(attacker, defender);

    attack = false;
}

//------------------------------------------------------------

void GameMechanics::paintDead(QPainter &painter)
{
    painter.setPen(Qt::black);
    painter.setBrush(Qt::DiagCrossPattern);

    for(Creature &creature : enemy)
        if(creature.isDead())
            painter.drawRect(creature.getPosition().x() * cellSize,
                             creature.getPosition().y() * cellSize,
                             cellSize,
                             cellSize);
    if(player->isDead())
        painter.drawRect(player->getPosition().x() * cellSize,
                         player->getPosition().y() * cellSize,
                         cellSize,
                         cellSize);
}

//------------------------------------------------------------

void GameMechanics::paint(int del)
{
    repaint();
    delay(del);
}

//------------------------------------------------------------

void GameMechanics::delay(int msec)
{
    /*QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);*/
}

//------------------------------------------------------------

void GameMechanics::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
        showToolTip(event);
    if (event->button() == Qt::LeftButton)
        selectCell(event);
    if (event->button() == Qt::RightButton)
        attackEnemy(event);

    event->accept();
}

//------------------------------------------------------------

void GameMechanics::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        movePlayer(event);

    event->accept();
}

//------------------------------------------------------------

void GameMechanics::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
        cellSize++;
    else
        if(cellSize > 1)
            cellSize--;
    repaint();

    event->accept();
}

//------------------------------------------------------------


void GameMechanics::showToolTip(QMouseEvent *event)
{
    QPoint pos = event->pos();
    QString text;

    int i = pos.x() / cellSize;
    int j = pos.y() / cellSize;

    bool hereIsCreature = false;

    for(Creature &creature : enemy)
        if (creature.position.x() == i &&
            creature.position.y() == j)
        {
            text = QString("(%1;%2)\nHP: %3/%4\n")
                    .arg(i).arg(j)
                    .arg(creature.getHP())
                    .arg(creature.getDNA().getGenValue(DNA::HP));
            for (int i = DNA::defenceFire; i < DNA::genTypeCount - 1; i++)
                text += strVector[i] + QString(": %1/%2\n")
                        .arg(creature.getDNA().getGenValue(i))
                        .arg(creature.getDNA().getGenMaxValue(i));
            text += strVector[DNA::actionpoints] +
                    QString(": %1/%2").arg(creature.getAP())
                    .arg(creature.getDNA().getGenValue(DNA::actionpoints));
            hereIsCreature = true;
            break;
        }

    if (player->position.x() == i &&
        player->position.y() == j)
    {
        text = QString("(%1;%2)\nHP: %3/%4\n")
                .arg(i).arg(j)
                .arg(player->getHP())
                .arg(player->getDNA().getGenValue(DNA::HP));
        for (int i = DNA::defenceFire; i < DNA::genTypeCount -1 ; i++)
            text += strVector[i] + QString(": %1/%2\n")
                    .arg(player->getDNA().getGenValue(i))
                    .arg(player->getDNA().getGenMaxValue(i));

        text += strVector[DNA::actionpoints] +
                QString(": %1/%2").arg(player->getAP())
                .arg(player->getDNA().getGenValue(DNA::actionpoints));
        hereIsCreature = true;
    }

    if (!hereIsCreature)
        text = QString(tr("(%1;%2)\n"
                      "CellType = %3")).arg(i).arg(j)
                                       .arg(int(map[i][j]));

    QToolTip::showText(this->mapToGlobal(pos), text, this);
}

//------------------------------------------------------------

void GameMechanics::selectCell(QMouseEvent *event)
{
    QPoint pos = event->pos();
    int i = pos.x() / cellSize;
    int j = pos.y() / cellSize;

    curCellPos = QPoint(i,j);
    repaint();
}

//------------------------------------------------------------

void GameMechanics::movePlayer(QMouseEvent *event)
{
    int x = event->pos().x() / cellSize;
    int y = event->pos().y() / cellSize;
    player->moveTo(x, y);
    repaint();
}

//------------------------------------------------------------

void GameMechanics::paintAttack(QPoint a, QPoint b, Damage::Type dT)
{
    QPoint corrector(cellSize/2, cellSize/2);
    attacker = a * cellSize;
    defender = b * cellSize;
    attacker += corrector;
    defender += corrector;
    damageType = dT;
    attack = true;
    repaint();
    delay(100);
    repaint();
}

//------------------------------------------------------------

void GameMechanics::attackEnemy(QMouseEvent *event)
{
    QPoint pos = event->pos();
    pos.rx() /= cellSize;
    pos.ry() /= cellSize;

    for (Creature &creature : enemy)
        if (pos == creature.getPosition())
        {
            player->attack(&creature, playersDT);
            break;
        }
}

//------------------------------------------------------------

bool GameMechanics::setPlayersDT(Damage::Type dt)
{
    if ((dt & Damage::Long) && (dt & Damage::Near))
        return false;

    playersDT = dt;
    return true;
}

//------------------------------------------------------------

void GameMechanics::enemyMove(Creature &creature)
{
    float distance = sqrtf(powf(creature.position.x() - player->position.x(), 2) +
                           powf(creature.position.y() - player->position.y(), 2));

    if (creature.getDNA().getGenValue(DNA::damageNear) >
        creature.getDNA().getGenValue(DNA::damageLong))
    {
        if(distance >= 20)
            creature.moveTo(player->position.x(), player->position.y());
        while(creature.getAP() > 0)
            if (creature.getDNA().getGenValue(DNA::damageFire) >
                creature.getDNA().getGenValue(DNA::damageIce))
                creature.attack(player, Damage::Type(Damage::Near | Damage::Fire));
            else
                creature.attack(player, Damage::Type(Damage::Near | Damage::Ice));
    }
    else
    {
        if(distance >= 40)
            creature.moveTo(player->position.x(), player->position.y());

        while(creature.getAP() > 0)
            if (creature.getDNA().getGenValue(DNA::damageFire) >
                creature.getDNA().getGenValue(DNA::damageIce))
                creature.attack(player, Damage::Type(Damage::Long | Damage::Fire));
            else
                creature.attack(player, Damage::Type(Damage::Long | Damage::Ice));
    }
}

//------------------------------------------------------------

void GameMechanics::nextMove()
{
    emit playersMove(false);

    allEnemiesDead = true;

    player->restoreAP();

    for(Creature &creature :  enemy)
        if (!creature.isDead())
        {
            enemyMove(creature);
            creature.storeDamage(currentDamage);
            currentDamage = 0;
            allEnemiesDead = false;

            creature.restoreAP();
        }

    if (allEnemiesDead || player->isDead())
        selection();
    emit playersMove(true);
}

//------------------------------------------------------------

void GameMechanics::storeDamage(int dmg)
{
    currentDamage += dmg;
}

//------------------------------------------------------------

void GameMechanics::selection()
{
    std::sort(enemy.begin(), enemy.end());

    generateMap(enemy[0].getDamageToPlayer());

    writeLog();

    for (unsigned i = enemy.size() / 2; i < enemy.size(); i++)
    {
        if (rand() % 2)
            enemy[i].getrDNA()->crossingover(enemy[i - enemy.size() / 2].getDNA(),
                                             rand() % DNA::genTypeCount);
        else
        {
            Creature tempCreature = enemy[i];
            enemy[i] = enemy[i - enemy.size() / 2];
            enemy[i].getrDNA()->crossingover(tempCreature.getDNA(),
                                             rand() % DNA::genTypeCount);
        }

        enemy[i].getrDNA()->randomMutation(DNA::genTypeCount);
        optimizeEnemy(enemy[i]);
    }

    for (Creature &creature : enemy)
    {
        creature.position = QPoint(rand() % mapSize, rand() % mapSize);
        creature.resetDamageToPlayer();
        creature.liven();
    }

    player->position = QPoint(rand() % mapSize, rand() % mapSize);

    emit newWave();
}

//------------------------------------------------------------

void GameMechanics::optimizeEnemy(Creature &creature)
{
    int damageP = creature.getDNA().getGenValue(DNA::damagePoints);

    int defF = player->getDNA().getGenValue(DNA::defenceFire) /
                player->getDNA().getGenValue(DNA::defencePoints);
    creature.getrDNA()->setGenValue(damageP * defF, DNA::damageFire);
    creature.getrDNA()->setGenValue(creature.getDNA().getGenValue(DNA::damagePoints) - defF,
                                    DNA::damageIce);

    int defL = player->getDNA().getGenValue(DNA::defenceLong) /
                player->getDNA().getGenValue(DNA::defencePoints);
    creature.getrDNA()->setGenValue(damageP * defL, DNA::damageLong);
    creature.getrDNA()->setGenValue(creature.getDNA().getGenValue(DNA::damagePoints) - defF,
                                    DNA::damageNear);
}

//------------------------------------------------------------

void GameMechanics::writeLog()
{
    QFile file("enemydatac.txt");
    QTextStream stream(&file);
    file.open(QIODevice::Append);

    for(unsigned i = 0; i < enemy.size(); i++)
        if(enemy[i].getDamageToPlayer() > 0)
        stream << QString(tr("enemy[%1]; DamN/I/L/F: %2/%3/%4/%5 dtP: %6 \r\n"))
                     .arg(i)
                     .arg(enemy[i].getDNA().getGenValue(DNA::damageNear))
                     .arg(enemy[i].getDNA().getGenValue(DNA::damageIce))
                     .arg(enemy[i].getDNA().getGenValue(DNA::damageLong))
                     .arg(enemy[i].getDNA().getGenValue(DNA::damageFire))
                     .arg(enemy[i].getDamageToPlayer());
    stream << tr("\r\n");

    file.close();
}
