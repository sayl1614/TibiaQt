#ifndef TILE_H
#define TILE_H

#include "character.h"
class Character;

#include <QPoint>
#include <QPixmap>
#include <QVector>
#include <QQueue>

#include <QPainter>

enum class TileSpeed{
    verySlow,
    slow,
    medium,
    fast,
    veryFast
};


class Tile{
public:
    Tile(const QPoint &screenCenter);

    void setTileImg(QPixmap *img);
    void addCharacter(Character *newChar);
    bool isWalkable();
    void setWalkable(bool value);
    bool isBusy(){return _isBusy;}
    void setTileBusy(bool value){_isBusy = value;}

    double getTileSpeed(int speed);

    void drawTile(QPoint &pos, double mapZoom, QPainter &painter);

    void drawTile(int x, int y, double mapZoom, QPainter &painter);
    void removeCharacter(Character *character);
    void drawCharacters(int x, int y, QPainter &painter);
    int getTileGarvity();
    bool hasCreature();
    QQueue<Character*> &getCharacters();

private:
    QPixmap *_tileImg;
    QQueue<Character*> _characters;

    bool _isWalkable = true;
    bool _isBusy = false;
    TileSpeed _tileGravity;

    const QPoint &_screenCenter;
};

#endif // TILE_H
