#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include "enums.h"

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

protected slots:
    virtual bool moveTwardsEnemy();
    virtual void meleeAttack();
};

#endif // ENEMY_H
