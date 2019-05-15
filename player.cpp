#include "player.h"

#include "mainwindow.h"

Player::Player(QString character, MainWindow *parent, int speed) :
    Character(character, parent, speed){
    _currHp = 2300;
    _maxHp = 2500;
}

void Player::attack(Character *enemy){

}

void Player::withinMelee(){

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
    int drawX = x - (_drawOffset * mapZoom) + _movement->getOffset().x();
    int drawY = y - (_drawOffset * mapZoom) + _movement->getOffset().y();

    _hpBar.draw(drawX + ((_drawOffset / 3) * mapZoom), drawY - 20, painter, mapZoom);

    _moveAnimation[(int)_direction]->draw(drawX,
                                          drawY,
                                          mapZoom, painter);
}



