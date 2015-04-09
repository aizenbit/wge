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

namespace DamageType{
enum DamageType {
    damageFire = 1,
    damageIce  = 2,
    damageLong = 4,
    damageNear = 8
};
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

