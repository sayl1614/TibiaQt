#include "tile.h"

Tile::Tile() :
    _tileImg(nullptr){
    init();
}
void Tile::init(){
    _isWalkable = true;
    _tileGravity = TileSpeed::medium;
}

void Tile::setTileImg(QPixmap *tileImg){
    _tileImg = tileImg;
}

void Tile::addCharacter(Character *newChar){
    _characters.push_back(newChar);
}

void Tile::removeCharacter(Character *character){
    for (int i = 0; i < _characters.size(); i++){
        if (_characters[i] == character){
            _characters.removeAt(i);
        }
    }
}

bool Tile::isWalkable(){return _isWalkable;}

bool Tile::hasCreature(){
    for (int i = 0; i < _characters.size(); i++){
        //if (_characters[i]->isMoving());
    }
    return _characters.size() > 0;
}


void Tile::setWalkable(bool value){_isWalkable = value;}

bool Tile::hasCharacter(Character *character){
    for (int i = 0; i < _characters.size(); i++){
        if (_characters[i] = character)
            return true;
    }
}

int Tile::getTileGarvity(){
    switch ((int)_tileGravity) {
    case (int)TileSpeed::verySlow:
        return 1000 - 119;
    case (int)TileSpeed::slow:
        return 1000 - 400;
    case (int)TileSpeed::medium:
        return 1000 - 600;
    case (int)TileSpeed::fast:
        return 1000 - 667;
    case (int)TileSpeed::veryFast:
        return 1000 - 857;
    }
}

// Returns ms per tile
double Tile::getTileSpeed(int speed){
    double multiplier;
    switch ((int)_tileGravity) {
    case (int)TileSpeed::verySlow:
        multiplier = 0.119;
        break;
    case (int)TileSpeed::slow:
        multiplier = 0.4;
        break;
    case (int)TileSpeed::medium:
        multiplier = 0.6;
        break;
    case (int)TileSpeed::fast:
        multiplier = 0.667;
        break;
    case (int)TileSpeed::veryFast:
        multiplier = 0.857;
        break;
    }
    return 60000 / ((speed * multiplier) + 0.5);
}

void Tile::drawTile(QPoint &pos, double mapZoom, QPainter &painter){
    drawTile(pos.x(), pos.y(), mapZoom, painter);
}
void Tile::drawTile(int x, int y, double mapZoom, QPainter &painter){
    painter.drawPixmap(x, y, _tileImg->width() * mapZoom, _tileImg->height() * mapZoom, *_tileImg);
}
void Tile::drawCharacters(int x, int y, QPainter &painter){
    for (int i = 0; i < _characters.size(); i++)
        _characters[i]->draw(x, y ,painter);
}
