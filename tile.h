#ifndef TILE_H
#define TILE_H

#include <assert.h>

#include "character.h"
class WorldMap;

#include <QPainter>
#include <QPoint>
#include <QPixmap>
#include <QVector>
#include <QQueue>


#include "enums.h"



class Tile{
public:
    Tile(QPixmap *image = nullptr, int tileWalkingTime = 100);
    Tile(QPoint pos, QPixmap *image = nullptr, int tileWalkingTime = 100);
    Tile(int x, int y, QPixmap *image = nullptr, int tileWalkingTime = 100);
    void init(int tileWalkingTime = 100);

    void setTileImg(QPixmap *img);
    void addCharacter(Character *newChar);
    bool isWalkable();
    void setWalkable(bool value);
    bool hasCharacter(Character *character);

    double getTileSpeed(int speed);
    QPoint getPos(){return _pos;}
    void setPos(QPoint pos){_pos = pos;}

    void drawTile(QPoint &pos, double mapZoom, QPainter &painter);

    void drawTile(int x, int y, double mapZoom, QPainter &painter);
    void removeCharacter(Character *character);
    void drawCharacters(int x, int y, QPainter &painter);
    int getTileGarvity();
    bool hasCreature();

    bool isBusy(){
        for (int i = 0; i < _characters.size(); i++){
            if (_characters[i]->getEnd() == this->_pos)
                return true;
        }
        return false;
    }

private:
    QPixmap *_tileImg;
    QQueue<Character *> _characters;

    bool _isWalkable = true;
    int _isBusy = 0;
    TileSpeed _tileWalkingTime;
    QPoint _pos;
};

#endif // TILE_H
