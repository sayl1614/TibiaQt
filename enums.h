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
    verySlow,
    slow,
    medium,
    fast,
    veryFast
};

#endif // ENUMS_H

