#include "worldmap.h"
WorldMap::WorldMap(MainWindow *parent) :
    _parent(parent){
    QPixmap *first = new QPixmap(":/images/tiles/1.png");
    QPixmap *second = new QPixmap(":/images/tiles/2.png");
    _theMap.resize(_mapHeight);
    for (int y = 0; y < _mapHeight; y++){
        for (int x = 0; x < _mapWidth; x++){
            _theMap[x].push_back(new Tile(_parent->movement.getScreenCenter()));
            _theMap[x][y]->setTileImg(first);
        }
    }

    _player = _parent->getPlayer();
    _theMap[_player->getPos()->x()][_player->getPos()->y()]->addCharacter(_player);
    /**/

    first = nullptr;
    second = nullptr;

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
    if (_theMap[point.x()][point.y()]->isWalkable())
        return true;
    return false;
}

Tile *WorldMap::getTile(int indx1, int indx2){
    return _theMap[indx1][indx2];
}

Tile *WorldMap::getTile(QPoint point){
    return getTile(point.x(), point.y());
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
    assert(!_player->isMoving());

    for (int i = 0; i < _mapMovementTimer.size(); i++){
        if (_mapMovementTimer[i]->isActive()){
            return;
        }
    }

    _mapMovementTimer[(int)direction]->start(_moveUpdateInMS);

    _msPerSquare = getSpeedPerTileForCharacter(_player);

    _player->setMovementAnimationInterval(_msPerSquare);

    _movementTime.restart();
}

void WorldMap::addCharacter(Character *obj){
    _theMap[obj->movement.getEndPos()->x()][obj->movement.getEndPos()->y()]->addCharacter(obj);
}
void WorldMap::addCharacter(int x, int y, Character *obj){
    _theMap[x][y]->addCharacter(obj);
}


void WorldMap::removeCharacter(Character *obj){
    _theMap[obj->movement.getStartPos()->x()][obj->movement.getStartPos()->y()]->removeCharacter(obj);
}
void WorldMap::removeCharacter(int x, int y, Character *obj){
    _theMap[x][y]->removeCharacter(obj);
}

QQueue<Character *> &WorldMap::getCharacters(QPoint pos){
    return getCharacters(pos.x(), pos.y());
}
QQueue<Character *> &WorldMap::getCharacters(int x, int y){
    return _theMap[x][y]->getCharacters();
}

void WorldMap::stopMoving(){
    addCharacter(_player);

    removeCharacter(_player);

    resetMovement(_player->getFacingDirection());
}

void WorldMap::resetMovement(FacingDirection direction){
    _mapMovementTimer[(int)direction]->stop();
    _player->stopMoving();
}

int WorldMap::getSpeedPerTileForCharacter(Character *character){
    return _theMap[character->getPos()->x()][character->getPos()->y()]->
            getTileSpeed(_player->getSpeed());
}

int WorldMap::getSpeedPerTileForCharacter(QPoint pos, Character *character){
    return _theMap[pos.x()][pos.y()]->getTileSpeed(_player->getSpeed());
}

void WorldMap::moveNorth(){
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
}

void WorldMap::moveWest(){
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
}

void WorldMap::moveSouth(){
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
}

void WorldMap::moveEast(){
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
}

int WorldMap::getIndexPos(char index){
    if (index == 'x')
        return floor((_parent->movement.getScreenPos()->x()) -
                              (_parent->movement.getScreenCenter().x() / _parent->dimentions.getMapZoom()));
    else // if 'x';
        return floor((_parent->movement.getScreenPos()->y()) -
                              (_parent->movement.getScreenCenter().y() / _parent->dimentions.getMapZoom()));
}

void WorldMap::drawMap(QPainter &painter){

    // Draw tiles (and items?)
    int indexPosY = getIndexPos('y');

    for (int drawYPos = -_parent->dimentions.getDrawTileSize() - _parent->movement.getOffsetY(); drawYPos < 850; drawYPos += _parent->dimentions.getDrawTileSize(), indexPosY++){
        if (indexPosY < 0 || indexPosY >= _mapHeight)
            continue;

        int indexPosX = getIndexPos('x');
        for (int drawXPos = -_parent->dimentions.getDrawTileSize() - _parent->movement.getOffsetX(); drawXPos < 1500; drawXPos += _parent->dimentions.getDrawTileSize(), indexPosX++){
            if (indexPosX < 0 || indexPosX >= _mapWidth)
                continue;


            _theMap[indexPosX][indexPosY]->drawTile( drawXPos, drawYPos,
                                                     _parent->dimentions.getMapZoom(), painter);
        }
    }

    // Draw characters
    indexPosY = getIndexPos('y');

    for (int drawYPos = -_parent->dimentions.getDrawTileSize() - _parent->movement.getOffsetY(); drawYPos < 850; drawYPos += _parent->dimentions.getDrawTileSize(), indexPosY++){
        if (indexPosY < 0 || indexPosY >= _mapHeight)
            continue;

        int indexPosX = getIndexPos('x');

        for (int drawXPos = -_parent->dimentions.getDrawTileSize() - _parent->movement.getOffsetX(); drawXPos < 1500; drawXPos += _parent->dimentions.getDrawTileSize(), indexPosX++){
            if (indexPosX < 0 || indexPosX >= _mapWidth)
                continue;

            _theMap[indexPosX][indexPosY]->drawCharacters(drawXPos, drawYPos, painter);
        }
    }
}
