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
    double mapZoom = _parent->dimentions.getMapZoom();
    _moveAnimation[(int)_direction]->draw(x - (_drawOffset * mapZoom) + _movement->getOffset().x(),
                                          y - (_drawOffset * mapZoom) + _movement->getOffset().y(),
                                          mapZoom, painter);
}



