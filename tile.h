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
    QQueue<Character *> &getCharacters();

private:
    QPixmap *_tileImg;
    QQueue<Character *> _characters;

    bool _isWalkable = true;
    bool _isBusy = false;
    TileSpeed _tileGravity;

};

#endif // TILE_H
