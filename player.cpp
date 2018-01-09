#include "player.h"

#include "mainwindow.h"

Player::Player(QString character, MainWindow *parent, int speed) :
    Character(character, parent, speed){
}

void Player::attack(Character *enemy){

}


void Player::draw(int x, int y, QPainter &painter){
    animationUpdate();
    setAnimationAttributes();
    painter.drawPixmap( x - _drawOffset + movement.getOffsetX(),
                        y - _drawOffset + movement.getOffsetY(),
                        _drawCharacterSize, _drawCharacterSize, *_player);
}



