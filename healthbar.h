#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <QPainter>
#include <QPoint>
#include <QPen>


class Healthbar{
public:
    Healthbar(int &currHp, int &maxHp);
    void draw(int x, int y, QPainter &painter, double mapZoom = 1);
private:
    int &_currHp;
    int &_maxHp;
};

#endif // HEALTHBAR_H
