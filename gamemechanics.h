#ifndef GAMEMECHANICS_H
#define GAMEMECHANICS_H

#include <QObject>

enum CellType {empty,
               grass,
               sand,
               wall,
               busyByPlayer,
               busyByEnemy,
               cellTypeCount};

//------------------------------------------------------------

class GameMechanics : public QObject
{
    Q_OBJECT

private:
    int mapSize {65}; //must be (power of 2) + 1
    CellType ** map;

    void diamondSquare(float **floatMap);
    void diamond(float **floatMap, int n);
    void square(float **floatMap, int n);

public:
    explicit GameMechanics(QObject *parent = 0);
    ~GameMechanics();

    int getMapSize() const;
    CellType getCell(int i, int j) const;

    void generateMap(unsigned int seed = 0);

signals:

public slots:
};

#endif // GAMEMECHANICS_H
