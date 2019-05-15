#include "worldmap.h"

#include <random>
#include <time.h>

WorldMap::WorldMap(MainWindow *parent) :
    _parent(parent){
    srand(time(NULL));
    QPixmap *firstImage = new QPixmap(":/images/tiles/1.png");
    QPixmap *secondImage = new QPixmap(":/images/tiles/green.png");
    QPixmap *thirdImage = new QPixmap(":/images/tiles/red.png");
    _theMap.resize(_mapHeight);
    for (int y = 0; y < _mapHeight; y++){
        for (int x = 0; x < _mapWidth; x++){
            int randoms = rand() % 3;
            if (randoms == 0)
                _theMap[x].push_back(new Tile(x, y, secondImage, 100));
            else if (randoms == 1)
                _theMap[x].push_back(new Tile(x, y, firstImage, 300));
            else
                _theMap[x].push_back(new Tile(x, y, thirdImage, 500));
        }
    }
    if (_parent->getPlayer()){
        _player = _parent->getPlayer();
        _theMap[_player->getEnd().x()][_player->getEnd().y()]->addCharacter(_player);
    }
    /**/

    firstImage = nullptr;
    secondImage = nullptr;

    int sides = 4;
    for (int i = 0; i < sides; i++)
        _mapMovementTimer.push_back(new QTimer(this));
    connect(_mapMovementTimer[(int)FacingDirection::north], SIGNAL(timeout()), this, SLOT(moveNorth()));
    connect(_mapMovementTimer[(int)FacingDirection::west], SIGNAL(timeout()), this, SLOT(moveWest()));
    connect(_mapMovementTimer[(int)FacingDirection::south], SIGNAL(timeout()), this, SLOT(moveSouth()));
    connect(_mapMovementTimer[(int)FacingDirection::east], SIGNAL(timeout()), this, SLOT(moveEast()));
    _movementTime.start();
}

bool WorldMap::isWalkable(int x, int y){
    return isWalkable(QPoint(x, y));
}

bool WorldMap::isWalkable(QPoint point){
    if (point.x() < 0 || point.y() < 0)
        return false;

    bool iswalkable = _theMap[point.x()][point.y()]->isWalkable();
    bool isbusy = isBusy(point);



    if (_theMap[point.x()][point.y()]->isWalkable() && !isBusy(point))
        return true;
    return false;
}

Tile *WorldMap::getTile(int indx1, int indx2){
    return _theMap[indx1][indx2];
}

Tile *WorldMap::getTile(QPoint point){
    return getTile(point.x(), point.y());
}


bool WorldMap::isBusy(QPoint pos){
    return isBusy(pos.x(), pos.y());
}
bool WorldMap::isBusy(int x, int y){
    return _theMap[x][y]->isBusy();
}

void WorldMap::toggleTile(int x, int y){
    if (_theMap[x][y]->isWalkable()){
        _theMap[x][y]->setTileImg(new QPixmap(":/images/tiles/black.png"));
        _theMap[x][y]->setWalkable(false);
    }
    else{
        _theMap[x][y]->setTileImg(new QPixmap(":/images/tiles/1.png"));
        _theMap[x][y]->setWalkable(true);
    }
}
void WorldMap::toggleTile(QPoint pos){
    toggleTile(pos.x(), pos.y());
}

bool WorldMap::valid(QPoint &pos){
    return valid(pos.x(), pos.y());
}
bool WorldMap::valid(QPoint *pos){
    return valid(pos->x(), pos->y());
}
bool WorldMap::valid(int x, int y){
    return (!(x >= _mapWidth ||
              x < 0 ||
              y >= _mapHeight ||
              y < 0));
}

void WorldMap::move(FacingDirection direction){
    //assert(!_player->isMoving());

    for (int i = 0; i < _mapMovementTimer.size(); i++){
        if (_mapMovementTimer[i]->isActive()){
            return;
        }
    }

    _mapMovementTimer[(int)direction]->start(_moveUpdateInMS);

    _msPerSquare = getSpeedPerTileForCharacter(_player);

    //_player->setMovementAnimationInterval(_msPerSquare);

    _movementTime.restart();
}

void WorldMap::addCharacter(Character *character){
    _theMap[character->getEnd().x()][character->getEnd().y()]->addCharacter(character);
}

void WorldMap::removeCharacter(Character *character){
    _theMap[character->getStart().x()][character->getStart().y()]->removeCharacter(character);
}
void WorldMap::stopMoving(){
    addCharacter(_player);

    removeCharacter(_player);

    //resetMovement(_player->getFacingDirection());
}

void WorldMap::resetMovement(FacingDirection direction){
    _mapMovementTimer[(int)direction]->stop();
    //_player->stopMoving();
}

int WorldMap::getSpeedPerTileForCharacter(Character *character){
    return _theMap[character->getEnd().x()][character->getEnd().y()]->
            getTileSpeed(character->getSpeed());
}

void WorldMap::moveNorth(){
    /*
    if (!_player->isMoving()){
        _player->movement.changeEndPos(0, -1);
        if (!valid(_player->movement.getEndPos())){
            _player->movement.setEndPos(_player->movement.getStartPos());
            resetMovement(FacingDirection::north);
            return;
        }
        _player->setFacingDirection(FacingDirection::north);
        _player->setState(State::moving);
    }
    double percent = _movementTime.elapsed() / (double)_msPerSquare;
    int dist = percent * _parent->dimentions.getDrawTileSize();

    _player->movement.changeOffset(0, -dist);

    if (_movementTime.elapsed() >= (_msPerSquare)){
        stopMoving();
    }
    */
}

void WorldMap::moveWest(){
    /*
    if (!_player->isMoving()){
        _player->movement.changeEndPos(-1, 0);
        if (!valid(_player->movement.getEndPos())){
            _player->movement.setEndPos(_player->movement.getStartPos());
            resetMovement(FacingDirection::west);
            return;
        }
        _player->setFacingDirection(FacingDirection::west);
        _player->setState(State::moving);
    }
    double percent = _movementTime.elapsed() / (double)_msPerSquare;
    int dist = percent * _parent->dimentions.getDrawTileSize();

    _player->movement.changeOffset(-dist, 0);

    if (_movementTime.elapsed() >= _msPerSquare){
        stopMoving();
    }
    */
}

void WorldMap::moveSouth(){
    /*
    if (!_player->isMoving()){
        _player->movement.changeEndPos(0, 1);
        if (!valid(_player->movement.getEndPos())){
            _player->movement.setEndPos(_player->movement.getStartPos());
            resetMovement(FacingDirection::south);
            return;
        }
        _player->setFacingDirection(FacingDirection::south);
        _player->setState(State::moving);
    }
    double percent = _movementTime.elapsed() / (double)_msPerSquare;
    int dist = percent * _parent->dimentions.getDrawTileSize();

    _player->movement.changeOffset(0, dist);

    if (_movementTime.elapsed() >= (_msPerSquare)){
        stopMoving();
    }
    */
}

void WorldMap::moveEast(){
    /*
    if (!_player->isMoving()){
        _player->movement.changeEndPos(1, 0);
        if (!valid(_player->movement.getEndPos())){
            _player->movement.setEndPos(_player->movement.getStartPos());
            resetMovement(FacingDirection::east);
            return;
        }
        _player->setFacingDirection(FacingDirection::east);
        _player->setState(State::moving);
    }
    double percent = _movementTime.elapsed() / (double)_msPerSquare;
    int dist = percent * _parent->dimentions.getDrawTileSize();

    _player->movement.changeOffset(dist, 0);

    if (_movementTime.elapsed() >= (_msPerSquare)){
        stopMoving();
    }
    */
}

int WorldMap::getIndexPos(char index){
    if (index == 'x')
        return floor((_parent->_screenPos->x()) -
                              (_parent->_screenCenter.x() / _parent->dimentions.getMapZoom()));
    else // if 'y';
        return floor((_parent->_screenPos->y()) -
                              (_parent->_screenCenter.y() / _parent->dimentions.getMapZoom()));
}

void WorldMap::drawMap(QPainter &painter){

    // Draw tiles (and items?)
    int indexPosY = getIndexPos('y');

    for (int drawYPos = -_parent->dimentions.getDrawTileSize() - _parent->_offset->y(); drawYPos < 850; drawYPos += _parent->dimentions.getDrawTileSize(), indexPosY++){
        if (indexPosY < 0 || indexPosY >= _mapHeight)
            continue;

        int indexPosX = getIndexPos('x');
        for (int drawXPos = -_parent->dimentions.getDrawTileSize() - _parent->_offset->x(); drawXPos < 1500; drawXPos += _parent->dimentions.getDrawTileSize(), indexPosX++){
            if (indexPosX < 0 || indexPosX >= _mapWidth)
                continue;


            _theMap[indexPosX][indexPosY]->drawTile( drawXPos, drawYPos, _parent->dimentions.getMapZoom(), painter);
        }
    }

    // Draw characters
    indexPosY = getIndexPos('y');
    for (int drawYPos = -_parent->dimentions.getDrawTileSize() - _parent->_offset->y(); drawYPos < 850; drawYPos += _parent->dimentions.getDrawTileSize(), indexPosY++){
        if (indexPosY < 0 || indexPosY >= _mapHeight)
            continue;

        int indexPosX = getIndexPos('x');
        for (int drawXPos = -_parent->dimentions.getDrawTileSize() - _parent->_offset->x(); drawXPos < 1500; drawXPos += _parent->dimentions.getDrawTileSize(), indexPosX++){
            if (indexPosX < 0 || indexPosX >= _mapWidth)
                continue;



            _theMap[indexPosX][indexPosY]->drawCharacters(drawXPos, drawYPos, painter);
        }
    }
}
