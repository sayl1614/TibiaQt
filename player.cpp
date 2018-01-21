#include "player.h"

#include "mainwindow.h"

Player::Player(QString character, MainWindow *parent, int speed) :
    Character(character, parent, speed){
}

void Player::attack(Character *enemy){

}

void Player::meleeAttack(){

}


void Player::noPath(bool tooFarAway){
    if (tooFarAway){
        // Stop follow. Remove follow markers
        return;
    }
    else{
        _moveAnimation[(int)_direction]->stop();
        return;
    }
}

void Player::draw(int x, int y, QPainter &painter){
    /*
    painter.drawPixmap( x - _drawOffset + movement.getOffsetX(),
                        y - _drawOffset + movement.getOffsetY(),
                        _drawCharacterSize, _drawCharacterSize, *_movementImages[]);


    _movementImages[movement.getAnimationIndex()]->draw(x, y, _parent->dimentions.getMapZoom(), painter);
    */
    _moveAnimation[(int)_direction]->draw(x - _drawOffset + _movement->getOffset().x(),
                                          y - _drawOffset + _movement->getOffset().y(),
                                          _parent->dimentions.getMapZoom(), painter);
}



