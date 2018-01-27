#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include "enums.h"

#include <QTimer>

class NPC : public Character{
public:
    NPC(QString character, MainWindow *parent, Character *enemy = nullptr, int speed = 250);

    void draw(int centerX, int centerY, QPainter &painter);

    void noPath(bool tooFarAway = false);

    void attack(Character *enemy);
    void withinMelee();



    ~NPC();
protected:
    void faceEnemy();
    void drawBox_Black(int x, int y, QPainter &painter, int offset = 0);

    bool _hasBox_Black = false;

    QTimer *_drawBox_BlackTimer;

protected slots:
    virtual bool moveTwardsEnemy();
    virtual void meleeAttack();
    void removeBlackBox();
};

#endif // ENEMY_H
