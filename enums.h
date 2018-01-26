#ifndef ENUMS_H
#define ENUMS_H

enum class State{
    none,
    moving,
    sleeping,
    eating
};
enum class FacingDirection{
    north,
    west,
    south,
    east,
    continueToNext,
    notFound
};


enum class TileSpeed{
    verySlow = 70,
    slow = 90,
    medium = 100,
    fast = 160,
    veryFast = 500
};

#endif // ENUMS_H

