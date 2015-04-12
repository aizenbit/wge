#ifndef GAMEMECHANICS_H
#define GAMEMECHANICS_H

#include <QtWidgets/QtWidgets>
#include <vector>

#include "creature.h"
#include "enum.h"

class GameMechanics : public QWidget
{
    Q_OBJECT

private:
    QColor cellColor[CellType::cellTypeCount] {
        QColor(0, 0, 0),
        QColor(152, 251, 152),
        QColor(253, 236, 143),
        QColor(130, 130, 130),
        QColor(0, 206, 209),
        QColor(255, 140, 0)};

    const int mapSize {65}; //must be (power of 2) + 1
    int cellSize {10};
    CellType::CellType ** map;
    QPoint curCellPos;

    std::vector<Creature> enemy;
    Creature *player;

    //for paintAttack
    QPoint attacker;
    QPoint defender;
    Damage::Type damageType;
    bool attack;

    Damage::Type playersDT;
    int currentDamage;
    bool allEnemiesDead;

    //generate map using Diamond-Square algorythm
    void diamondSquare(float **floatMap);
    void diamond(float **floatMap, int n);
    void square(float **floatMap, int n);

    //paint all that can be painted
    virtual void paintEvent(QPaintEvent *);
    void paintMap(QPainter &painter);
    void paintEnemy(QPainter &painter);
    void paintPlayer(QPainter &painter);
    void paintWay(QPainter &painter, const Creature &creature);
    void paintCelectedCell(QPainter &painter);
    void paintAttack(QPainter &painter);
    void paintDead(QPainter &painter);

    void delay(int msec);

    //mouse and wheel events
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    void showToolTip(QMouseEvent *);
    void selectCell(QMouseEvent *);
    void movePlayer(QMouseEvent *);
    void attackEnemy(QMouseEvent *);
    void wheelEvent(QWheelEvent *);

    //
    void enemyMove(Creature &creature);
    void selection();

private slots:
    void storeDamage(int);

public:
    QVector <QString> strVector;

    explicit GameMechanics(QWidget *parent = 0);
    ~GameMechanics();

    int getMapSize() const;
    QSize getMinimumSize() const;
    CellType::CellType getCell(int i, int j) const;
    const Creature getPlayer() const;
    Creature* getrPlayer();


    void generateMap(unsigned int seed = 0);


signals:
    void newWave();

public slots:
    void paint(int del);
    void paintAttack(QPoint a, QPoint b, Damage::Type dT);
    bool setPlayersDT(Damage::Type dt);
    void nextMove();
};

#endif // GAMEMECHANICS_H
