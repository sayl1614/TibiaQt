#include "movement.h"

#include "character.h"
#include "worldmap.h"
#include "mainwindow.h"

Movement::Movement(Character *character) :
    _character(character){
    srand(time(NULL));
    init();
}

void Movement::init(){
    _state = State::none;
    _msPerSquare = 0;
    _timeSpentMoving = new QElapsedTimer();
    _timeSpentMoving->start();

    _followTimer = new QTimer(this);
    connect(_followTimer, SIGNAL(timeout()), this, SLOT(follow()));

    int directions = 4;
    for (int i = 0; i < directions; i++){
        _moveInterval.push_back(new QTimer(this));
    }
    connect(_moveInterval[0], SIGNAL(timeout()), this, SLOT(moveNorth()));
    connect(_moveInterval[1], SIGNAL(timeout()), this, SLOT(moveWest()));
    connect(_moveInterval[2], SIGNAL(timeout()), this, SLOT(moveSouth()));
    connect(_moveInterval[3], SIGNAL(timeout()), this, SLOT(moveEast()));
}

void Movement::changeNext(int x, int y){
    _next.setX(_next.x() + x);
    _next.setY(_next.y() + y);
}

void Movement::changeOffset(int x, int y){
    _offset.setX(_offset.x() + x);
    _offset.setY(_offset.y() + y);
}

void Movement::meleeDistance(){
    _followTimer->start(2000);
    if (_character->targetIsHostile()){
        _character->meleeAttack();
        _runExhausted = true;
    }
}

void Movement::follow(){
    if (_character->hasTarget()){
        _followTimer->stop();
        if (!_character->withinChasing()){ // If too far away
            _state = State::none;
            _followTimer->start(500);
            _character->noPath(true);
            return;
        }
        else if (_character->distanceToEnemy() == 0){ // Still in melee distance
            _state = State::none;
            meleeDistance();
            return;
        }

        FacingDirection direction = _character->findPath();

        if (direction == FacingDirection::notFound){// No path!
            _character->noPath();
            return;
        }
        move(direction);

        if(_character->distanceToEnemy() > 0) { // Still not within melee
            if (isMoving()){ // continue following
                return;
            }
            else
                return;
        }
        else{ // Just arrived at melee distance
            if (_character->targetIsHostile()){
                meleeDistance();
            }
            else
                _followTimer->start(500);
        }
        return;
    }
    assert(!_followTimer->isActive());
}

void Movement::move(FacingDirection direction){
    WorldMap *worldMap = _character->getWorldMap();
    _msPerSquare = worldMap->getSpeedPerTileForCharacter(_character);

    if (direction == FacingDirection::notFound){
        return;
    }
    for (int i = 0; i < _moveInterval.size(); i++){
        if (_moveInterval[i]->isActive()){
            return;
        }
    }
    _runExhausted = false;
    switch (direction) {
    case FacingDirection::north:
        changeNext(0, -1);
        if (!worldMap->isWalkable(_next)){
            _next = _start;
            _moveInterval[(int)direction]->stop();
            return;
        }
        _character->face(direction);
        break;
    case FacingDirection::west:
        changeNext(-1, 0);
        if (!worldMap->isWalkable(_next)){
            _next = _start;
            _moveInterval[(int)direction]->stop();
            return;
        }
        _character->face(direction);
        break;
    case FacingDirection::south:
        changeNext(0, 1);
        if (!worldMap->isWalkable(_next)){
            _next = _start;
            _moveInterval[(int)direction]->stop();
            return;
        }
        _character->face(direction);
        break;
    case FacingDirection::east:
        changeNext(1, 0);
        if (!worldMap->isWalkable(_next)){
            _next = _start;
            _moveInterval[(int)direction]->stop();
            return;
        }
        _character->face(direction);
        break;
    }
    if (!isMoving())
        _state = State::moving;

    worldMap->addCharacter(_character);

    _moveInterval[(int)direction]->start(_moveUpdateInMS);
    _character->playAnimation(_msPerSquare);

    _timeSpentMoving->restart();
    return;
}



void Movement::moveNorth(){

    MainWindow *gui = _character->getGui();
    int timeSpentMoving = _timeSpentMoving->elapsed();
    double percent = timeSpentMoving / (double)_msPerSquare;

    int dist = percent * gui->dimentions.getDrawTileSize();

    setOffset(0, -dist);

    if (timeSpentMoving >= (_msPerSquare)){
        stopMoving();
    }
}

void Movement::moveWest(){
    MainWindow *gui = _character->getGui();
    int timeSpentMoving = _timeSpentMoving->elapsed();
    double percent = timeSpentMoving / (double)_msPerSquare;

    int dist = percent * gui->dimentions.getDrawTileSize();

    setOffset(-dist, 0);

    if (timeSpentMoving >= _msPerSquare){
        stopMoving();
    }
}

void Movement::moveSouth(){
    MainWindow *gui = _character->getGui();
    int timeSpentMoving = _timeSpentMoving->elapsed();
    double percent = timeSpentMoving / (double)_msPerSquare;

    int dist = percent * gui->dimentions.getDrawTileSize();

    setOffset(0, dist);

    if (timeSpentMoving >= (_msPerSquare)){
        stopMoving();
    }
}

void Movement::moveEast(){
    MainWindow *gui = _character->getGui();
    int timeSpentMoving = _timeSpentMoving->elapsed();
    double percent = timeSpentMoving / (double)_msPerSquare;

    int dist = percent * gui->dimentions.getDrawTileSize();

    setOffset(dist, 0);

    if (timeSpentMoving >= (_msPerSquare)){
        stopMoving();
    }
}

void Movement::stopMoving(){
    setOffset(0, 0);
    for (int i = 0; i < _moveInterval.size(); i++){
        if (_moveInterval[i]->isActive()){
            _moveInterval[i]->stop();
        }
    }
    WorldMap *worldMap = _character->getWorldMap();
    worldMap->removeCharacter(_character);
    _start = _next;

    if (_isFollowing){
        if (isMoving() || _runExhausted){
            _state = State::none;
            _character->stopAnimation();
        }
        if (!_followTimer->isActive())
            follow();
    }
    else{
        _state = State::none;
        _character->stopAnimation();
    }
}


int Movement::movementWonderAround(){
    FacingDirection direction = (FacingDirection)(rand() % 4);
    move(direction);
    if (isMoving())
        _followTimer->start(_msPerSquare * 3);
    else
        _followTimer->start(_msPerSquare + 25);
}
