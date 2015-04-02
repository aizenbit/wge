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
/*char** cellTypeText = {
    "empty",
    "grass",
    "sand",
    "wall",
    "busyByPlayer",
    "busyByEnemy",
    "cellTypeCount"
};*/
}

namespace Direction {
enum Direction {
    nope = 0,
    up = -1,
    down = 1,
    left = -1,
    right = 1
};
}

#endif // ENUM_H

