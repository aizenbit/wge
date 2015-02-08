#ifndef GAMEMECHANICS_H
#define GAMEMECHANICS_H

#include <QObject>

enum CellType {empty,
               wall,
               busyByPlayer,
               busyByEnemy,
               cellTypeCount};

//------------------------------------------------------------

class GameMechanics : public QObject
{
    Q_OBJECT

private:
    int mapSize {8};
    CellType ** map;

public:
    explicit GameMechanics(QObject *parent = 0);
    ~GameMechanics();

    int getMapSize() const;
    CellType getCell(int i, int j) const;

signals:

public slots:
};

#endif // GAMEMECHANICS_H
