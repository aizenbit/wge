#ifndef ENUM_H
#define ENUM_H

namespace CellType {
enum CellType {
    empty,
    grass,
    sand,
    wall,
    busyByPlayer,
    busyByEnemy,
    cellTypeCount
};
}

namespace Direction {
enum Direction {
    up,
    down,
    left,
    right
};
}

#endif // ENUM_H

