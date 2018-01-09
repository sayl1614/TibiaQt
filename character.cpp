#include "character.h"

#include "mainwindow.h"

Character::Character(QString character, MainWindow *parent, int speed) :
    _moveSpeed(speed), _parent(parent){

    srand(time(NULL));

    movement.setAnimationIndex(0);
    movement._move.resize(4);

    loadCharacterImages(character);
    setFacingDirection(FacingDirection::south);
    setAnimationAttributes();


    int sides = 4;
    for (int i = 0; i < sides; i++)
        _mapMovementTimer.push_back(new QTimer(this));
    connect(_mapMovementTimer[(int)FacingDirection::north], SIGNAL(timeout()), this, SLOT(moveNorth()));
    connect(_mapMovementTimer[(int)FacingDirection::west], SIGNAL(timeout()), this, SLOT(moveWest()));
    connect(_mapMovementTimer[(int)FacingDirection::south], SIGNAL(timeout()), this, SLOT(moveSouth()));
    connect(_mapMovementTimer[(int)FacingDirection::east], SIGNAL(timeout()), this, SLOT(moveEast()));
    _movementTime.start();

    _followTimer = new QTimer(this);
    connect(_followTimer, SIGNAL(timeout()), this, SLOT(follow()));

    _box_BlackTimer = new QTimer(this);
    connect(_box_BlackTimer, SIGNAL(timeout()), this, SLOT(setBox_BlackOff()));

    _movementAnimationTimer.start();
}

// rerun when level up?
void Character::setAnimationAttributes(){
    _orgCharacterSize = _player->width();
    _drawCharacterSize = _parent->dimentions.getMapZoom() * _orgCharacterSize;
    _drawTileSize = _orgTileSize * _parent->dimentions.getMapZoom();
    _drawOffset = _drawCharacterSize - _drawTileSize;
}


void Character::animateNextMovement(){
    if (movement.getAnimationIndex() >= movement._move[(int)_facingDir].size()){
        int newPos = movement._move[(int)_facingDir].size() > 2 ? 1 : 0;
        movement.setAnimationIndex(newPos);
    }
    _player = &movement._move[(int)_facingDir][movement.getAnimationIndex()];
}

void Character::animationUpdate(){
    if ((isMoving() || hasPassiveAnimation()) &&
            (_movementAnimationTimer.elapsed() > movement.getMovementAnimationInterval())){
        movement++;
        animateNextMovement();
        _movementAnimationTimer.restart();
    }
}

void Character::directionUpdate(){
    _player = &movement._move[(int)_facingDir][movement.getAnimationIndex()];
}

void Character::setCharacter(QString newChar){
    while (movement._move[(int)FacingDirection::north].size())
        movement._move[(int)FacingDirection::north].pop_back();
    while (movement._move[(int)FacingDirection::west].size())
        movement._move[(int)FacingDirection::west].pop_back();
    while (movement._move[(int)FacingDirection::south].size())
        movement._move[(int)FacingDirection::south].pop_back();
    while (movement._move[(int)FacingDirection::east].size())
        movement._move[(int)FacingDirection::east].pop_back();
    loadCharacterImages(newChar);
}

void Character::stopMoving(){
    _state = State::none;
    movement.setAnimationIndex(0);
    directionUpdate();
    movement.changeOffset(0, 0);

    _parent->addCharacter(movement.getEndPos(), this);
    _parent->removeCharacter(movement.getStartPos(), this);

    movement.setStartPos(movement.getEndPos());
    for (int i = 0; i < _mapMovementTimer.size(); i++){
        if (_mapMovementTimer[i]->isActive()){
            _mapMovementTimer[i]->stop();
        }
    }
}

int Character::move(FacingDirection direction){
    if (direction == FacingDirection::notFound)
        return -1;
    for (int i = 0; i < _mapMovementTimer.size(); i++){
        if (_mapMovementTimer[i]->isActive()){
            return -1;
        }
    }
    switch (direction) {
    case FacingDirection::north:
        movement.changeEndPos(0, -1);
        if (!_parent->isWalkable(*movement.getEndPos())){
            movement.setEndPos(movement.getStartPos());
            return -1;
        }
        setFacingDirection(FacingDirection::north);
        break;
    case FacingDirection::west:
        movement.changeEndPos(-1, 0);
        if (!_parent->isWalkable(*movement.getEndPos())){
            movement.setEndPos(movement.getStartPos());
            return -1;
        }
        setFacingDirection(FacingDirection::west);
        break;
    case FacingDirection::south:
        movement.changeEndPos(0, 1);
        if (!_parent->isWalkable(*movement.getEndPos())){
            movement.setEndPos(movement.getStartPos());
            return -1;
        }
        setFacingDirection(FacingDirection::south);
        break;
    case FacingDirection::east:
        movement.changeEndPos(1, 0);
        if (!_parent->isWalkable(*movement.getEndPos())){
            movement.setEndPos(movement.getStartPos());
            _mapMovementTimer[(int)direction]->stop();
            return -1;
        }
        setFacingDirection(FacingDirection::east);
        break;
    }
    _parent->getWorldMap()->getTile(*movement.getEndPos())->setTileBusy(true);
    _parent->getWorldMap()->getTile(*movement.getStartPos())->setTileBusy(false);
    setState(State::moving);

    _mapMovementTimer[(int)direction]->start(this->_moveUpdateInMS);

    _msPerSquare = _parent->getSpeedPerTileForCharacter(this);

    setMovementAnimationInterval(_msPerSquare);

    _movementTime.restart();
    return _msPerSquare;
}

void Character::moveNorth(){
    double percent = _movementTime.elapsed() / (double)_msPerSquare;
    int dist = percent * _parent->dimentions.getDrawTileSize();

    movement.changeOffset(0, -dist);

    if (_movementTime.elapsed() >= (_msPerSquare)){
        stopMoving();
    }
}

void Character::moveWest(){
    double percent = _movementTime.elapsed() / (double)_msPerSquare;
    int dist = percent * _parent->dimentions.getDrawTileSize();

    movement.changeOffset(-dist, 0);

    if (_movementTime.elapsed() >= _msPerSquare){
        stopMoving();
    }
}

void Character::moveSouth(){
    double percent = _movementTime.elapsed() / (double)_msPerSquare;
    int dist = percent * _parent->dimentions.getDrawTileSize();

    movement.changeOffset(0, dist);

    if (_movementTime.elapsed() >= (_msPerSquare)){
        stopMoving();
    }
}

void Character::moveEast(){
    double percent = _movementTime.elapsed() / (double)_msPerSquare;
    int dist = percent * _parent->dimentions.getDrawTileSize();

    movement.changeOffset(dist, 0);

    if (_movementTime.elapsed() >= (_msPerSquare)){
        stopMoving();
    }
}

int Character::distanceToEnemy(QPoint start, QPoint end){
    int distanceX = (abs(start.x() - end.x()));
    int distanceY = (abs(start.y() - end.y()));

    distanceX += distanceX > 0 ? - 1 : 0;
    distanceY += distanceY > 0 ? - 1 : 0;

    return distanceX + distanceY;
}

bool withinChasing(QPoint start, QPoint end){
    if (abs(start.x() - end.x()) < 9 &&
            abs(start.y() - end.y()) < 7)
        return true;
    return false;
}

void Character::movementWonderAround(){
    int speed = move((FacingDirection)(rand() % 4));
    _followTimer->start(speed + rand() % 1000);
}

void Character::follow(){
    if (_enemy){
        if (!withinChasing(*this->getPos(), *_enemy->getPos()))
            movementWonderAround();
        int speed = move(_parent->findPath(this->getPos(), _enemy->getPos()));

        if(distanceToEnemy(*this->getPos(), *_enemy->getPos()) > 0) {
            if (speed < 0){ // No path!
                movementWonderAround();
            }
                else{
                _followTimer->start(speed + this->_moveUpdateInMS + 5);
            }
        }
        if (distanceToEnemy(*this->getPos(), *_enemy->getPos()) == 0) { // Melee distance
            meleeAttack(speed);
        }
    }
}

void Character::meleeAttack(int speed){
}



void Character::loadCharacterImages(QString character){
    QPixmap temp;
    int counter = 1;
    while (temp.load(":/images/characters/" + character + "/north/" + QString::number(counter++) + ".png")){
        movement._move[(int)FacingDirection::north].push_back(temp);
    }
    counter = 1;
    while (temp.load(":/images/characters/" + character + "/west/" + QString::number(counter++) + ".png")){
        movement._move[(int)FacingDirection::west].push_back(temp);
    }
    counter = 1;
    while (temp.load(":/images/characters/" + character + "/south/" + QString::number(counter++) + ".png")){
        movement._move[(int)FacingDirection::south].push_back(temp);
    }
    counter = 1;
    while (temp.load(":/images/characters/" + character + "/east/" + QString::number(counter++) + ".png")){
        movement._move[(int)FacingDirection::east].push_back(temp);
    }
}

void Character::drawBox_Black(int x, int y, QPainter &painter, int avvika){
    int penWidth = 6;
    QPen pen;
    pen.setWidth(penWidth);
    painter.setPen(pen);

    x += _drawTileSize;
    y += _drawTileSize;

    int xLeft = x + avvika;
    int xRight = x + _drawTileSize - avvika - penWidth;
    int yTop = y + avvika;
    int yBottom = y  + _drawTileSize - avvika - penWidth;

    painter.drawLine(xLeft, yTop, xRight, yTop);
    painter.drawLine(xRight, yTop, xRight, yBottom);
    painter.drawLine(xRight, yBottom, xLeft, yBottom);
    painter.drawLine(xLeft, yBottom, xLeft, yTop);
}

void Character::drawBox_Red(int x, int y, QPainter &painter, int avvika){
    int penWidth = 6;
    QPen pen;
    pen.setWidth(penWidth);
    pen.setColor(qRgb(255,0,0));

    painter.setPen(pen);

    int xLeft = x + avvika;
    int xRight = x + _drawTileSize - avvika - penWidth;
    int yTop = y + avvika;
    int yBottom = y + _drawTileSize - avvika - penWidth;

    painter.drawLine(xLeft, yTop, xRight, yTop);
    painter.drawLine(xRight, yTop, xRight, yBottom);
    painter.drawLine(xRight, yBottom, xLeft, yBottom);
    painter.drawLine(xLeft, yBottom, xLeft, yTop);

    pen.setColor(qRgb(0,0,0));
    painter.setPen(pen);
}

void Character::setFacingDirection(FacingDirection dir){
    _facingDir = dir;
    animateNextMovement();
}

void Character::setMovementAnimationInterval(int msPerSquare){
    movement.setMovementAnimationInterval(msPerSquare / movement.getFramesPerSquare());
}

