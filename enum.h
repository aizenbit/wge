#ifndef ENUM_H
#define ENUM_H

enum CellType {
    empty,
    grass,
    sand,
    wall,
    busyByPlayer,
    busyByEnemy,
    cellTypeCount
};

enum Direction {
    up,
    down,
    left,
    right
};

#endif // ENUM_H

