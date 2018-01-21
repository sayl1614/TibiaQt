#include "npc.h"
#include "mainwindow.h"


NPC::NPC(QString character, MainWindow *parent, Character *enemy, int speed) :
    Character(character, parent, speed){
    _target = enemy;
    if (_target){
        follow(_target);
    }
}

void NPC::draw(int x, int y, QPainter &painter){
    _moveAnimation[(int)_direction]->draw(x - _drawOffset + _movement->getOffset().x(),
                                          y - _drawOffset + _movement->getOffset().y(),
                                          _parent->dimentions.getMapZoom(), painter);
}

void NPC::noPath(bool tooFarAway){
    stopAnimation();
    if (tooFarAway){
        return;
    }
    _movement->movementWonderAround();
}

void NPC::attack(Character *enemy){
    _isHostile = true;
}

void NPC::meleeAttack(){
    //setBox_BlackOn();
    // Do an attack inbetween

    // Do an attack inbetween

    if (isMoving())
        return;

    bool moveNext = rand() % 100 < 33; // 33% chance to move

    if (this->getEnd().x() < _target->getEnd().x()) { // west of target
        int direction = (int)rand() % 2;
        if (moveNext) {
            if (this->getEnd().y() < _target->getEnd().y()){       // north west of target
                if (direction == 0)
                    move(FacingDirection::east);
                else
                    move(FacingDirection::south);
            }
            else if (this->getEnd().y() > _target->getEnd().y()){  // south west of target
                if (direction == 0)
                    move(FacingDirection::east);
                else
                    move(FacingDirection::north);
            }
            else{                                                   // West of target
                if (direction == 0)
                    move(FacingDirection::north);
                else
                    move(FacingDirection::south);
            }
            if (!isMoving()) {    // failed
                face(FacingDirection::east);
            }
        }
        else { // Just turn twards enemy
            if (!isMoving()){
                face(FacingDirection::east);
            }
        }
    }

    else if (this->getEnd().x() > _target->getEnd().x()) { // east of target
        if (moveNext) {
            int direction = (int)rand() % 2;
            if (this->getEnd().y() < _target->getEnd().y()){       // north east of target
                if (direction == 0)
                    move(FacingDirection::west);
                else
                    move(FacingDirection::south);
            }
            else if (this->getEnd().y() > _target->getEnd().y()){  // south east of target
                if (direction == 0)
                    move(FacingDirection::west);
                else
                    move(FacingDirection::north);
            }
            else{                                                   // east of target
                if (direction == 0)
                    move(FacingDirection::north);
                else
                    move(FacingDirection::south);
            }

            if (!isMoving()) {    // failed
                face(FacingDirection::west);
            }
        }
        else { // Just turn twards enemy
            if (!isMoving()){
                face(FacingDirection::west);
            }
        }
    }
    else if (this->getEnd().y() < _target->getEnd().y()) {         // north of target
        if (moveNext){
            int direction = (int)rand() % 2;
            if (direction == 0)
                move(FacingDirection::west);
            else
                move(FacingDirection::east);
            if (!isMoving()) {    // failed
                face(FacingDirection::south);
            }
        }
        else { // Just turn twards enemy
            if (!isMoving()){
                face(FacingDirection::south);
            }
        }
    }
    else if (this->getEnd().y() > _target->getEnd().y()) {          // south of target                                                          // south of target
        if (moveNext) {
            int direction = (int)rand() % 2;
            if (direction == 0)
                move(FacingDirection::west);
            else
                move(FacingDirection::east);
            if (!isMoving()) {    // failed
                face(FacingDirection::north);
            }
        }
        else { // Just turn twards enemy
            if (!isMoving()){
                face(FacingDirection::north);
            }
        }
    }
    else {                                                          // ontop of target
        moveNext = rand() % 100 < 60; // 60% chance to move
        if (moveNext){
            FacingDirection direction = (FacingDirection)(rand() % 4);
            move(direction);
        }
    }
    //_followTimer->start(2000);
}




NPC::~NPC(){

}


/*
void Npc::draw(int x, int y, QPainter &painter){
    animationUpdate();
    setAnimationAttributes();
    if (_hasBox_Black)
        drawBox_Black(x - _drawOffset + movement.getOffsetX(), y - _drawOffset + movement.getOffsetY(), painter, 0);

    painter.drawPixmap( x - _drawOffset + movement.getOffsetX(),
                        y - _drawOffset + movement.getOffsetY(),
                        _drawCharacterSize, _drawCharacterSize, this->_movementImages[movement._animationIndex]);
}

void NPC::attack(Character *enemy){
    if (!_enemy){
        _enemy = enemy;
        _followTimer->start(0);
    }
    else{
        _enemy = nullptr;
        _followTimer->stop();
    }
}




*/
bool NPC::moveTwardsEnemy(){

}
