#include "character.h"

#include "mainwindow.h"


Character::Character(QString character, MainWindow *parent, int speed) :
    _charName(character), _parent(parent){

    _movement = new Movement(this);
    _movement->setSpeed(speed);

    _meleeTimer = new QTimer(this);
    connect(_meleeTimer, SIGNAL(timeout()), this, SLOT(meleeAttack()));
    _timeSinceLastAttack.start();

    init(character);
}

void Character::init(QString charName){
    _moveAnimation.push_back(new Image(":/images/characters/" + charName + "/north"));
    _moveAnimation.push_back(new Image(":/images/characters/" + charName + "/west"));
    _moveAnimation.push_back(new Image(":/images/characters/" + charName + "/south"));
    _moveAnimation.push_back(new Image(":/images/characters/" + charName + "/east"));
    _drawOffset = _moveAnimation[0]->getWidth() - _parent->dimentions.getDrawTileSize();
}

WorldMap *Character::getWorldMap(){
    return _parent->getWorldMap();
}

#include <tuple>
std::tuple<QPoint*, QPoint*> Character::getPosReference(){
    return std::make_tuple(_movement->getStartRef(), _movement->getOffsetRef());
}

void Character::reload(QString character){
    for (int i = 0; i < _moveAnimation.size(); i++){
        _moveAnimation[i]->reloadImage(character);
    }
}

void Character::face(FacingDirection direction){
    stopAnimation();
    _direction = direction;
}

void Character::draw(int x, int y, QPainter &painter) {
    _moveAnimation[(int)_direction]->draw(x, y, _parent->dimentions.getMapZoom(), painter);
}

void Character::move(FacingDirection direction){
    _movement->move(direction);
}

void Character::stopMoving(){
    _movement->stopMoving();
}

FacingDirection Character::findPath(){
    return findPath(_target);
}

FacingDirection Character::findPath(Character *target){
    return _parent->findPath(getEnd(), target->getEnd());
}

FacingDirection Character::findPath(QPoint pos){
    return _parent->findPath(getEnd(), pos);
}

void Character::follow(Character *target){
    if (_target){
        _movement->setFollowing(false);
        _target = nullptr;
        _movement->stopMoving();
    }
    else{
        _target = target;
        _movement->setFollowing(true);
    }
    _movement->follow();
}

int Character::distanceToEnemy(){
    int distanceX = (abs(getEnd().x() - _target->getEnd().x()));
    int distanceY = (abs(getEnd().y() - _target->getEnd().y()));

    distanceX += distanceX > 0 ? - 1 : 0;
    distanceY += distanceY > 0 ? - 1 : 0;

    return distanceX + distanceY;
}

bool Character::withinChasing(){
    if (abs(getEnd().x() - _target->getEnd().x()) < 9 &&
            abs(getEnd().y() - _target->getEnd().y()) < 7)
        return true;
    return false;
}

void Character::playAnimation(int _msPerSquare){
    _moveAnimation[(int)_direction]->play(_msPerSquare);
}

void Character::stopAnimation(){
    _moveAnimation[(int)_direction]->stop();
    _moveAnimation[(int)_direction]->reset();
}
