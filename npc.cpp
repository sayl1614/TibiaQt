#include "npc.h"
#include "mainwindow.h"


#include <QPen>
#include <QBrush>

#include <QDebug>


NPC::NPC(QString character, MainWindow *parent, Character *enemy, int speed) :
    Character(character, parent, speed){

    _drawBox_BlackTimer = new QTimer(this);
    connect(_drawBox_BlackTimer, SIGNAL(timeout()), this, SLOT(removeBlackBox()));

    _target = enemy;
    if (_target){
        _movement->setFollowing(true);
        follow();
    }
}

void NPC::draw(int x, int y, QPainter &painter){
    double mapZoom = _parent->dimentions.getMapZoom();
    int drawX = x - (_drawOffset * mapZoom)  + _movement->getOffset().x();
    int drawY = y - (_drawOffset * mapZoom)  + _movement->getOffset().y();


    _hpBar.draw(drawX + ((_drawOffset / 3) * mapZoom), drawY - 20, painter, mapZoom);

    if (_hasBox_Black)
        drawBox_Black(x, y, painter, 0);

    _moveAnimation[(int)_direction]->draw(drawX, drawY, mapZoom, painter);
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

void NPC::faceEnemy(){
    if (this->getEnd().x() < _target->getEnd().x()) {       // west of target
        face(FacingDirection::east);
    }
    else if (this->getEnd().x() > _target->getEnd().x()) {  // east of target
        face(FacingDirection::west);
    }
    else if (this->getEnd().y() < _target->getEnd().y()) {  // north of target
        face(FacingDirection::south);
    }
    else if (this->getEnd().y() > _target->getEnd().y()) {  // south of target
        face(FacingDirection::north);
    }
}

void NPC::removeBlackBox(){
    _hasBox_Black = false;
}

void NPC::drawBox_Black(int x, int y, QPainter &painter, int offset){
    double mapZoom = _parent->dimentions.getMapZoom();

    QPen pen;
    pen.setWidth(7 * mapZoom);

    painter.setPen(pen);

    painter.drawRect(x + _movement->getOffset().x(),
                     y + _movement->getOffset().y(),
                     _parent->dimentions.getDrawTileSize(), _parent->dimentions.getDrawTileSize());

}

void NPC::withinMelee(){
    //setBox_BlackOn();
    // Do an attack inbetween
    if (!_meleeTimer->isActive())
        meleeAttack();
    // Do an attack inbetween


    // ok to remove?
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
        }
    }
    else if (this->getEnd().y() < _target->getEnd().y()) {         // north of target
        if (moveNext){
            int direction = (int)rand() % 2;
            if (direction == 0)
                move(FacingDirection::west);
            else
                move(FacingDirection::east);

        }
    }
    else if (this->getEnd().y() > _target->getEnd().y()) {          // south of target                                                          // south of target
        if (moveNext) {
            int direction = (int)rand() % 2;
            if (direction == 0)
                move(FacingDirection::west);
            else
                move(FacingDirection::east);
        }
    }
    else {                                                          // ontop of target
        moveNext = rand() % 100 < 60; // 60% chance to move
        if (moveNext){
            FacingDirection direction = (FacingDirection)(rand() % 4);
            move(direction);
        }
    }
}


bool NPC::moveTwardsEnemy(){

}

void NPC::meleeAttack(){
    if (distanceToEnemy() > 0){
        _meleeTimer->stop();
        return;
    }
    _meleeTimer->start(2000);
    _movement->setFollowTimer(2000);
    //attack
    if (!isMoving()){
        faceEnemy();
    }
    _hasBox_Black = true;
    _drawBox_BlackTimer->start(1000);
    _target->drawBlood();
}


NPC::~NPC(){

}
