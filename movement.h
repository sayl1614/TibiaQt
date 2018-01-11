#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "enums.h"

#include <QObject>
#include <QString>
#include <QTimer>

#include <QPoint>

#include <QVector>

#include <QPainter>

class WorldMap;
class Character;


class Movement : public QObject{
    Q_OBJECT
public:
    Movement(Character *character);

    void init();

    int move(FacingDirection direction, WorldMap *character);

    QPoint getStart(){return _start;}
    void setStart(QPoint pos){_start = pos;}
    QPoint getNext(){return _next;}
    void setNext(QPoint pos){_next = pos;}
    void changeNext(int x, int y);

    int getSpeed(){return _speed;}
    int setSpeed(int speed){_speed = speed;}

    ~Movement(){}
private:
    Character *_character;

    int _moveUpdateInMS = 20;

    QVector<QTimer *> _moveInterval;
    int _msPerSquare;

    QPoint _start;
    QPoint _next;
    QPoint _offset {0, 0};

    State _state;
    int _speed;
private slots:
    void moveNorth();
    void moveWest();
    void moveSouth();
    void moveEast();
};

#endif // MOVEMENT_H
