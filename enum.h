#ifndef ENUM_H
#define ENUM_H

//Enums

enum class CellType {
    empty,
    grass,
    sand,
    wall,
    busyByPlayer,
    busyByEnemy,
    cellTypeCount
};


enum class DamageType : short {
    None = 0,
    Fire = 1,
    Ice  = 2,
    Long = 4,
    Near = 8
};

enum class Direction {
    nope = 0,
    up = -1,
    down = 1,
    left = -1,
    right = 1
};


//Overloaded operators special for enums
static DamageType operator | (DamageType l, DamageType r)
{
    return static_cast<DamageType>(static_cast<short>(l) |
                      static_cast<short>(r));
}

static bool operator & (DamageType l, DamageType r)
{
    return static_cast<bool>(static_cast<short>(l) &
                      static_cast<short>(r));
}

static bool operator && (DamageType l, DamageType r)
{
    return static_cast<bool>(static_cast<short>(l) &&
                      static_cast<short>(r));
}

static int operator + (CellType l, int r)
{
    return static_cast<int>(static_cast<int>(l) + r);
}

static int operator - (CellType l, int r)
{
    return static_cast<int>(static_cast<int>(l) - r);
}

static int operator + (int l, Direction r)
{
    return static_cast<int>(static_cast<int>(l) + r);
}

static int operator += (int& l, Direction r)
{
    l = static_cast<int>(static_cast<int>(l) + r);
    return l;
}

#endif // ENUM_H

