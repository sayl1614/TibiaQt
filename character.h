#ifndef CHARACTER_H
#define CHARACTER_H

#include "enums.h"
#include "movement.h"
#include "image.h"

#include <QObject>
#include <QPoint>
#include <QVector>

#include <QTimer>
#include <QElapsedTimer>

#include "healthbar.h"

class MainWindow;


class Character : public QObject{
    Q_OBJECT
public:
    Character(QString character, MainWindow *parent, int speed = 220);

    void init(QString charName);

    WorldMap *getWorldMap();
    MainWindow *getGui(){return _parent;}

    // Positioning
    virtual QPoint getStart(){return _movement->getStart();}
    virtual QPoint getEnd(){return _movement->getNext();}
    virtual QPoint getOffset(){return _movement->getOffset();}
    std::tuple<QPoint*, QPoint*> getPosReference();


    // Movement
    virtual void move(FacingDirection direction);
    virtual void stopMoving();
    virtual bool isMoving(){return _movement->isMoving();}
    virtual FacingDirection findPath();
    virtual FacingDirection findPath(Character *target);
    virtual FacingDirection findPath(QPoint pos);
    void follow(Character *target = nullptr);
    void setState(State state){_movement->setState(state);}

    int distanceToEnemy();

    bool withinChasing();

    virtual void noPath(bool tooFarAway = false) = 0;
    virtual int getSpeed(){return _movement->getSpeed();}


    // Combat
    bool hasTarget(){return (bool)_target;}
    bool targetIsHostile(){return _isHostile;}
    Character *getTarget(){return _target;}
    virtual void withinMelee() = 0;
    void drawBlood(){_currHp -= 10;}

    virtual void playAnimation(int _msPerSquare);
    virtual void stopAnimation();
    virtual void reload(QString character);

    virtual void face(FacingDirection direction);


    virtual void draw(int x, int y, QPainter &painter);

    virtual ~Character(){
        // Remove when dynamic!!
        while (_moveAnimation.size()){
            delete _moveAnimation[_moveAnimation.size() - 1];
            _moveAnimation.pop_back();
        }
        delete _movement;
    }
    void toggleFollow();
protected slots:
    virtual void meleeAttack() = 0;
    virtual void removeBlackBox(){}
protected:
    QString _charName;

    MainWindow *_parent;

    Character *_target;
    bool _isHostile = true;

    int _framesPerSquare = 8;
    QVector<Image*> _moveAnimation;
    FacingDirection _direction {FacingDirection::south};
    Movement *_movement;

    QTimer *_meleeTimer;
    QElapsedTimer _timeSinceLastAttack;


    int _currHp = 250;
    int _maxHp = 250;
    Healthbar _hpBar {_currHp, _maxHp};

    int _drawOffset;
};

#endif // CHARACTER_H
