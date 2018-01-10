#ifndef CHARACTER_H
#define CHARACTER_H

#include "image.h"

#include <QString>
#include <QPoint>

// Drawing
#include <QPixmap>
#include <QPainter>


// Containers
#include <QVector>
#include <QQueue>

// Timers
#include <QElapsedTimer>
#include <QTimer>

#include <random>
#include <time.h>


#include <QDebug>

#include "enums.h"

class MainWindow;


class Character : public QObject{
    Q_OBJECT
public:
    Character(QString character, MainWindow *parent, int speed = 220);
    QVector<Image*> _movementImages;
    virtual void draw(int x, int y, QPainter &painter) = 0;

    virtual void setState(State state){_state = state;}
    virtual State getState(){return _state;}

    virtual void attack(Character *enemy) = 0;



    virtual void animateNextMovement();
    virtual bool hasPassiveAnimation(){return _hasPassiveAnimation;}
    virtual void animationUpdate();
    void setAnimationAttributes();


    virtual void reloadCharacter(QString newChar);

    void drawTheMarks(QPainter &painter);


    QPoint *getPos(){
        if (_state == State::moving)
            return movement.getEndPos();
        else
            return movement.getStartPos();
    }

    struct Movement{

        void changePos(int x, int y){
            setStartPos(x, y);
            setEndPos(x, y);
        }
        void changePos(QPoint pos){
            changePos(pos.x(), pos.y());
        }
        void changePos(QPoint *pos){
            changePos(pos->x(), pos->y());
        }

        void setStartPos(int x, int y){_startPos.setX(x); _startPos.setY(y);}
        void setStartPos(QPoint pos){_startPos = pos;}
        void setStartPos(QPoint *pos){setStartPos(pos->x(), pos->y());}
        void changeStartPos(int x, int y){
            _startPos.setX(_startPos.x() + x);
            _startPos.setY(_startPos.y() + y);
        }
        QPoint *getStartPos(){return &_startPos;}


        void setEndPos(QPoint pos){_endPos = pos;}
        void setEndPos(QPoint *pos){setEndPos(pos->x(), pos->y());}
        void setEndPos(int x, int y){_endPos.setX(x); _endPos.setY(y);}
        void changeEndPos(int x, int y){
            _endPos.setX(_endPos.x() + x);
            _endPos.setY(_endPos.y() + y);
        }
        QPoint *getEndPos(){return &_endPos;}

        QPoint *getOffset(){return &_offset;}
        int getOffsetX(){return _offset.x();}
        int getOffsetY(){return _offset.y();}
        void changeOffset(int x, int y){_offset.setX(x); _offset.setY(y);}

        int getAnimationIndex(){return _animationIndex;}
        void setAnimationIndex(int value){_animationIndex = value;}

        int getMovementAnimationInterval(){return _movementAnimationInterval;}
        void setMovementAnimationInterval(int value){_movementAnimationInterval = value;}

        int getFramesPerSquare(){return _framesPerSquare;}
        void setFramesPerSquare(int value){_framesPerSquare = value;}


        void operator ++(int){_animationIndex++;}

        QVector< QVector< QPixmap>> _move;
        QQueue<QPoint>_nextSquare;

        /**/

        /**/
    private:
        int _framesPerSquare = 8;
        int _animationIndex;
        int _movementTimeToNextAnimation;
        int _movementAnimationInterval;

        QPoint _startPos {5, 5};
        QPoint _endPos {5, 5};
        QPoint _offset {0, 0};
    } movement;

    virtual ~Character(){
        if (_parent)
            delete _parent;
        for (int i = 0; i < _movementImages.size(); i++)
            delete _movementImages[i];
    }


    // Movement
    virtual void setFacingDirection(FacingDirection dir);
    virtual FacingDirection getFacingDirection(){return _facingDir;}
    int move(FacingDirection direction);
    virtual bool isMoving(){return _state == State::moving;}
    virtual void stopMoving();
    void resetMovement(FacingDirection direction);
    virtual int getSpeed(){return _moveSpeed;}
    void setMovementAnimationInterval(int msPerSquare);


    int distanceToEnemy(QPoint start, QPoint end);
    void drawBox_Black(int x, int y, QPainter &painter, int avvika);
    void drawBox_Red(int x, int y, QPainter &painter, int avvika);
    void setBox_BlackOn(int time = 1000){_hasBox_Black = true; _box_BlackTimer->start(time);}
    virtual void meleeAttack(int speed) = 0;
    void movementWanderAround();
    bool withinMelee(QPoint start, QPoint end);
    bool withinChasingDistance(QPoint start, QPoint end);
protected slots:
    void moveNorth();
    void moveWest();
    void moveSouth();
    void moveEast();
    void follow();
    void setBox_BlackOff(){_hasBox_Black = false; _box_BlackTimer->stop();}

protected:
    // Combat
    bool _hasBox_Black  = false;
    QTimer *_box_BlackTimer;

    // private movement
    int _moveUpdateInMS = 20;
    int _msPerSquare;

    QElapsedTimer _movementAnimationTimer;
    int _animationSpeed;

    QVector<QTimer*> _mapMovementTimer;
    QTimer *_followTimer;
    QElapsedTimer _movementTime;

    // Combat start

    // Combat end

    // Attributes
    QPixmap *_player;
    int _moveSpeed;
    int _hp;
    int _ap;
    int _critChance;


    virtual void directionUpdate();
    QQueue<FacingDirection>_movingDirection;
    FacingDirection _facingDir;
    State _state {State::none};


    // Drawing
    int _drawOffset;

    bool _hasPassiveAnimation = false;

    int _orgTileSize = 100;
    int _drawTileSize;
    int _orgCharacterSize;
    int _drawCharacterSize;



    Character *_enemy = nullptr;
    MainWindow *_parent;


private:
    virtual void loadCharacterImages(QString character);
protected slots:

};

#endif // CHARACTER_H
