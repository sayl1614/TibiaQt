#include "movement.h"

#include "character.h"
#include "worldmap.h"

Movement::Movement(Character *character) :
    _character(character){
    int directions = 4;
    for (int i = 0; i < directions; i++){
        _moveInterval.push_back(new QTimer(this));
    }
    connect(_moveInterval[0], SIGNAL(timeout()), this, SLOT(moveNorth()));
    connect(_moveInterval[1], SIGNAL(timeout()), this, SLOT(moveWest()));
    connect(_moveInterval[2], SIGNAL(timeout()), this, SLOT(moveSouth()));
    connect(_moveInterval[3], SIGNAL(timeout()), this, SLOT(moveEast()));

    init();
}

void Movement::init(){
    _state = State::none;
    _msPerSquare = 0;
}

void Movement::changeNext(int x, int y){
    _next.setX(_next.x() + x);
    _next.setY(_next.y() + y);
}

int Movement::move(FacingDirection direction, WorldMap *theMap){
    _character->setDirection(direction);
    if (direction == FacingDirection::notFound)
        return -1;

    for (int i = 0; i < _moveInterval.size(); i++){
        if (_moveInterval[i]->isActive()){
            return -1;
        }
    }
    switch (direction) {
    case FacingDirection::north:
        changeNext(0, -1);
        if (!theMap->isWalkable(_next)){
            _next = _start;
            break;
        }
        _character->face(FacingDirection::north);
        break;
    case FacingDirection::west:
        changeNext(-1, 0);
        if (!theMap->isWalkable(_next)){
            _next = _start;
            break;
        }
        _character->face(FacingDirection::west);
        break;
    case FacingDirection::south:
        changeNext(0, 1);
        if (!theMap->isWalkable(_next)){
            _next = _start;
            break;
        }
        _character->face(FacingDirection::south);
        break;
    case FacingDirection::east:
        changeNext(1, 0);
        if (!theMap->isWalkable(_next)){
            _next = _start;
            break;
        }
        _character->face(FacingDirection::east);
        break;
    }
    if (_next == _start){
        _moveInterval[(int)direction]->stop();
        return -1;
    }
    theMap->isBusy(_next);
    _state = State::moving;

    _moveInterval[(int)direction]->start(_moveUpdateInMS);
    _msPerSquare = theMap->getSpeedPerTileForCharacter(_character);
    _character->playAnimation(_msPerSquare);


    //_movementTime.restart();
    return _msPerSquare;
}

void Movement::moveNorth(){

}

void Movement::moveWest(){

}

void Movement::moveSouth(){

}

void Movement::moveEast(){

}

