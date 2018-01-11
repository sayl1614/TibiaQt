#ifndef WORLDMAP_H
#define WORLDMAP_H
#include <assert.h>
#include <QDebug>

#include <stdlib.h> // abs() function

#include "tile.h"
#include <vector>
#include <queue>
#include <functional>
#include <QString>

#include <QElapsedTimer>

#include "character.h"
class WorldMap;

#include <QTimer>

#include <QPainter>


#include "enums.h"

#include "mainwindow.h"

class MainWindow;

class WorldMap : public QObject{
    Q_OBJECT
public:
    WorldMap(MainWindow *parent);
    void drawMap(QPainter &painter);

    bool isWalkable(int x, int y);
    bool isWalkable(QPoint point);

    Tile *getTile(int indx1, int indx2);
    Tile *getTile(QPoint point);

    bool isBusy(int x, int y);
    bool isBusy(QPoint pos);

    void toggleTile(int x, int y); // Ta bort sen?
    void toggleTile(QPoint pos); // Ta bort sen?




    bool valid(QPoint &pos);
    bool valid(QPoint *pos);
    bool valid(int x, int y);
    void move(FacingDirection direction);
    void stopMoving();
    int getSpeedPerTileForCharacter(Character *character);

    int getMapWidth(){return _mapWidth;}
    int getMapHeight(){return _mapHeight;}

    // Draw
    int getIndexPos(char index);

    void resetMovement(FacingDirection direction);

    void addCharacter(Character *newChar);
    void addCharacter(int x, int y, Character *obj);
    void removeCharacter(Character *other);
    void removeCharacter(int x, int y, Character *obj);

private slots:
    void moveNorth();
    void moveWest();
    void moveSouth();
    void moveEast();
private:
    // Movement
    int _moveUpdateInMS = 20;
    int _msPerSquare;


    QVector<QTimer*> _mapMovementTimer;
    QElapsedTimer _movementTime;

    int _mapWidth = 100;
    int _mapHeight = 100;
    QVector<QVector<Tile*>> _theMap;

    Character *_player;
    MainWindow *_parent;
};

#endif // WORLDMAP_H
