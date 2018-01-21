#ifndef PLAYER_H
#define PLAYER_H
#include "character.h"


#include "enums.h"


class Player : public Character{
public:

    Player(QString character, MainWindow *parent, int speed = 500);

    void attack(Character *enemy);
    void draw(int centerX, int centerY, QPainter &painter);
    void meleeAttack();
    void noPath(bool tooFarAway = false);


    ~Player(){}
protected:

protected slots:
};

#endif // PLAYER_H
