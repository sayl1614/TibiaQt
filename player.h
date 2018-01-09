#ifndef PLAYER_H
#define PLAYER_H
#include "character.h"


class Player : public Character{
    Q_OBJECT
public:
    Player(QString character, MainWindow *parent, int speed = 220);

    void attack(Character *enemy);
    void draw(int centerX, int centerY, QPainter &painter);


    ~Player(){}
protected:

protected slots:
};

#endif // PLAYER_H
