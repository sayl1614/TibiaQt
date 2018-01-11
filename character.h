#ifndef CHARACTER_H
#define CHARACTER_H

#include "enums.h"
#include "movement.h"
#include "image.h"

#include <QObject>
#include <QPoint>
#include <QVector>

class MainWindow;


class Character : public QObject{
    Q_OBJECT
public:
    Character(QString character, MainWindow *parent, int speed = 220);

    void init();

    State isMoving(){return _state;}
    QPoint getStart();
    QPoint getEnd();

    int getSpeed(){return 1;}

    void setDirection(FacingDirection direction){_direction = direction;}

    void move(FacingDirection direction);


    void playAnimation(int _msPerSquare);
    void reload(QString character);

    void face(FacingDirection direction);


    void draw(int x, int y, double zoom, QPainter &painter);


private:
    MainWindow *_parent;

    int _framesPerSquare = 8;
    QVector<Image*> _moveAnimation;
    FacingDirection _direction {FacingDirection::south};
    //Movement movement;


    State _state;

    QPoint _start;
    QPoint _end;
};

#endif // CHARACTER_H
