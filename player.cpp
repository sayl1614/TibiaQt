#include "player.h"

#include "mainwindow.h"

Player::Player(QString character, MainWindow *parent, int speed) :
    Character(character, parent, speed){
}

void Player::attack(Character *enemy){

}

void Player::meleeAttack(int speed){

}

void Player::draw(int x, int y, QPainter &painter){
    /*
    painter.drawPixmap( x - _drawOffset + movement.getOffsetX(),
                        y - _drawOffset + movement.getOffsetY(),
                        _drawCharacterSize, _drawCharacterSize, *_movementImages[]);


    _movementImages[movement.getAnimationIndex()]->draw(x, y, _parent->dimentions.getMapZoom(), painter);
    */
}



