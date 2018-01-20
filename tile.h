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
    Tile();
    void init();

    void setTileImg(QPixmap *img);
    void addCharacter(Character *newChar);
    bool isWalkable();
    void setWalkable(bool value);
    bool hasCharacter(Character *character);

    double getTileSpeed(int speed);

    void drawTile(QPoint &pos, double mapZoom, QPainter &painter);

    void drawTile(int x, int y, double mapZoom, QPainter &painter);
    void removeCharacter(Character *character);
    void drawCharacters(int x, int y, QPainter &painter);
    int getTileGarvity();
    bool hasCreature();

    bool isBusy(){return _isBusy;}
    void addIsBusy(){_isBusy++;}
    void removeIsBusy(){_isBusy--;}

private:
    QPixmap *_tileImg;
    QQueue<Character *> _characters;

    bool _isWalkable = true;
    int _isBusy = 0;
    TileSpeed _tileGravity;

};

#endif // TILE_H
