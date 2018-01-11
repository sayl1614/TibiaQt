#include "character.h"

#include "mainwindow.h"


Character::Character(QString character, MainWindow *parent, int speed) :
    _parent(parent){

    //movement.setSpeed(speed);

    _moveAnimation.push_back(new Image(":/images/characters/" + character + "/north"));
    _moveAnimation.push_back(new Image(":/images/characters/" + character + "/west"));
    _moveAnimation.push_back(new Image(":/images/characters/" + character + "/south"));
    _moveAnimation.push_back(new Image(":/images/characters/" + character + "/east"));
}

void Character::init(){

}

void Character::reload(QString character){
    for (int i = 0; i < _moveAnimation.size(); i++){
        _moveAnimation[i]->reloadImage(character);
    }
}

void Character::face(FacingDirection direction){
    _direction = direction;
}

void Character::draw(int x, int y, double zoom, QPainter &painter) {
    _moveAnimation[(int)_direction]->draw(x, y, zoom, painter);
}


QPoint Character::getStart(){

}

QPoint Character::getEnd(){

}

void Character::move(FacingDirection direction){
    setDirection(direction);
    playAnimation(50);
}

void Character::playAnimation(int _msPerSquare){
    _moveAnimation[(int)_direction]->play(_msPerSquare);
}
