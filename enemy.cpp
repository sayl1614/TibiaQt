#include "enemy.h"
#include "mainwindow.h"

Enemy::Enemy(QString character, MainWindow *parent, Character *enemy, int speed) :
    Character("demon", parent, speed){
    _enemy = enemy;
    if (_enemy){
        _followTimer->start(0);
    }
}

void Enemy::draw(int x, int y, QPainter &painter){
    animationUpdate();
    setAnimationAttributes();
    if (_hasBox_Black)
        drawBox_Black(x - _drawOffset + movement.getOffsetX(), y - _drawOffset + movement.getOffsetY(), painter, 0);

    painter.drawPixmap( x - _drawOffset + movement.getOffsetX(),
                        y - _drawOffset + movement.getOffsetY(),
                        _drawCharacterSize, _drawCharacterSize, this->_movementImages[movement._animationIndex]);
}

void Enemy::attack(Character *enemy){
    if (!_enemy){
        _enemy = enemy;
        _followTimer->start(0);
    }
    else{
        _enemy = nullptr;
        _followTimer->stop();
    }
}

void Enemy::meleeAttack(int speed){
    setBox_BlackOn();
    // Do an attack inbetween

    // Do an attack inbetween
    bool moveNext = rand() % 100 < 33; // 25% chance to move

    if (this->getPos()->x() < _enemy->getPos()->x()) { // west of target
        if (moveNext) {
            if (this->getPos()->y() < _enemy->getPos()->y()){       // north west of target
                int direction = rand() % 2;
                if (direction == 0)
                    speed = move(FacingDirection::east);
                else
                    speed = move(FacingDirection::south);
            }
            else if (this->getPos()->y() > _enemy->getPos()->y()){  // south west of target
                int direction = (int)rand() % 2;
                if (direction == 0)
                    speed = move(FacingDirection::east);
                else
                    speed = move(FacingDirection::north);
            }
            else{                                                   // West of target
                int direction = (int)rand() % 2;
                if (direction == 0)
                    speed = move(FacingDirection::north);
                else
                    speed = move(FacingDirection::south);
            }

            if (speed < 0 && !isMoving()) {    // failed
                setFacingDirection(FacingDirection::east);
                directionUpdate();
            }
        }
        else { // Just turn twards enemy
            if (!isMoving()){
                setFacingDirection(FacingDirection::east);
                directionUpdate();
            }
        }
    }

    else if (this->getPos()->x() > _enemy->getPos()->x()) { // east of target
        if (moveNext) {
            if (this->getPos()->y() < _enemy->getPos()->y()){       // north east of target
                int direction = rand() % 2;
                if (direction == 0)
                    speed = move(FacingDirection::west);
                else
                    speed = move(FacingDirection::south);
            }
            else if (this->getPos()->y() > _enemy->getPos()->y()){  // south east of target
                int direction = (int)rand() % 2;
                if (direction == 0)
                    speed = move(FacingDirection::west);
                else
                    speed = move(FacingDirection::north);
            }
            else{                                                   // east of target
                int direction = (int)rand() % 2;
                if (direction == 0)
                    speed = move(FacingDirection::north);
                else
                    speed = move(FacingDirection::south);
            }

            if (speed < 0 && !isMoving()) {    // failed
                setFacingDirection(FacingDirection::west);
                directionUpdate();
            }
        }
        else { // Just turn twards enemy
            if (!isMoving()){
                setFacingDirection(FacingDirection::west);
                directionUpdate();
            }
        }
    }
    else if (this->getPos()->y() < _enemy->getPos()->y()) {         // north of target
        if (moveNext){
            int direction = (int)rand() % 2;
            if (direction == 0)
                speed = move(FacingDirection::west);
            else
                speed = move(FacingDirection::east);
            if (speed < 0 && !isMoving()) {    // failed
                setFacingDirection(FacingDirection::south);
                directionUpdate();
            }
        }
        else { // Just turn twards enemy
            if (!isMoving()){
                setFacingDirection(FacingDirection::south);
                directionUpdate();
            }
        }
    }
    else if (this->getPos()->y() > _enemy->getPos()->y()) {          // south of target                                                          // south of target
        if (moveNext) {
            int direction = (int)rand() % 2;
            if (direction == 0)
                speed = move(FacingDirection::west);
            else
                speed = move(FacingDirection::east);
            if (speed < 0 && !isMoving()) {    // failed
                setFacingDirection(FacingDirection::north);
                directionUpdate();
            }
        }
        else { // Just turn twards enemy
            if (!isMoving()){
                setFacingDirection(FacingDirection::north);
                directionUpdate();
            }
        }
    }
    else {                                                          // ontop of target
        moveNext = rand() % 100 < 60; // 50% chance to move
        if (moveNext){
            FacingDirection direction = (FacingDirection)(rand() % 4);
            speed = move(direction);
        }
    }
    _followTimer->start(2000);
}



bool Enemy::moveTwardsEnemy(){

}


