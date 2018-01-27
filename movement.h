#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "enums.h"

#include <QObject>
#include <QString>
#include <QTimer>
#include <QElapsedTimer>

#include <QPoint>

#include <QVector>

#include <QPainter>

#include <random>
#include <time.h>

class WorldMap;
class Character;


class Movement : public QObject{
    Q_OBJECT
public:
    Movement(Character *character);

    void init();

    QPoint getStart(){return _start;}
    QPoint *getStartRef(){return &_start;}
    void setStart(QPoint pos){_start = pos;}
    QPoint getNext(){return _next;}
    void setNext(QPoint pos){_next = pos;}
    void changeNext(int x, int y);
    QPoint getOffset(){return _offset;}
    QPoint *getOffsetRef(){return &_offset;}
    void setOffset(QPoint pos){_offset = pos;}
    void setOffset(int x, int y){setOffset(QPoint(x, y));}
    void changeOffset(int x, int y);

    void setState(State state){_state = state;}
    void setFollowing(bool value){_isFollowing = value;}
    void setFollowTimer(int value){_followTimer->start(value);}

    int movementWonderAround();

    int getSpeed(){return _speed;}
    int setSpeed(int speed){_speed = speed;}


    void move(FacingDirection direction);
    bool isMoving(){return _state == State::moving;}
    void stopMoving();

    ~Movement(){}
    void meleeAttack();
private:
    Character *_character;

    int _moveUpdateInMS = 20;

    QVector<QTimer *> _moveInterval;
    QTimer *_followTimer;
    QElapsedTimer *_timeSpentMoving;
    int _msPerSquare;

    QPoint _start {5, 5};
    QPoint _next {5, 5};
    QPoint _offset {0, 0};

    State _state;
    bool _isFollowing = false;
    bool _runExhausted = false;

    int _speed;
public slots:
    void follow();
private slots:
    void moveNorth();
    void moveWest();
    void moveSouth();
    void moveEast();
};

#endif // MOVEMENT_H
