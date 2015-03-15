#ifndef GAMEMECHANICS_H
#define GAMEMECHANICS_H

#include <QtWidgets/QtWidgets>

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
        QColor(220, 220, 220),
        QColor(0, 206, 209),
        QColor(255, 140, 0)};

    const int mapSize {65}; //must be (power of 2) + 1
    CellType::CellType ** map;

    void diamondSquare(float **floatMap);
    void diamond(float **floatMap, int n);
    void square(float **floatMap, int n);

public:
    explicit GameMechanics(QWidget *parent = 0);
    ~GameMechanics();

    int getMapSize() const;
    CellType::CellType getCell(int i, int j) const;

    void generateMap(unsigned int seed = 0);
    virtual void paintEvent(QPaintEvent *);

signals:

public slots:
};

#endif // GAMEMECHANICS_H
