#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"

class Enemy : public Character{
public:
    Enemy(QString character, MainWindow *parent, Character *enemy = nullptr, int speed = 100);

    void draw(int centerX, int centerY, QPainter &painter);

    void attack(Character *enemy);
    void meleeAttack(int speed);

    ~Enemy(){}
private:
protected slots:
    virtual bool moveTwardsEnemy();
};

#endif // ENEMY_H
