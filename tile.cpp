#include "tile.h"

Tile::Tile(QPixmap *image, int tileWalkingTime) :
    _tileImg(image){
    init(tileWalkingTime);
}

Tile::Tile(QPoint pos, QPixmap *image, int tileWalkingTime) :
    _pos(pos), _tileImg(image){
    init(tileWalkingTime);
}

Tile::Tile(int x, int y, QPixmap *image, int tileWalkingTime) :
    _tileImg(image){
    _pos.setX(x);
    _pos.setY(y);
    init(tileWalkingTime);
}

void Tile::init(int tileWalkingTime){
    _isWalkable = true;
    _tileWalkingTime = (TileSpeed)tileWalkingTime;
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
        if (_characters[i]->getEnd() == _pos)
            return true;
    }
    return false;
}


void Tile::setWalkable(bool value){_isWalkable = value;}

bool Tile::hasCharacter(Character *character){
    for (int i = 0; i < _characters.size(); i++){
        if (_characters[i] = character)
            return true;
    }
}

int Tile::getTileGarvity(){
    return (int)_tileWalkingTime;
}

// Returns ms per tile
double Tile::getTileSpeed(int speed){
    return 1000 * ((double)_tileWalkingTime / speed);
}

void Tile::drawTile(QPoint &pos, double mapZoom, QPainter &painter){
    drawTile(pos.x(), pos.y(), mapZoom, painter);
}
void Tile::drawTile(int x, int y, double mapZoom, QPainter &painter){
    painter.drawPixmap(x, y, _tileImg->width() * mapZoom, _tileImg->height() * mapZoom, *_tileImg);
}
void Tile::drawCharacters(int x, int y, QPainter &painter){
    for (int i = 0; i < _characters.size(); i++)
        if (_characters[i]->getStart() == this->_pos)
            _characters[i]->draw(x, y ,painter);
}
