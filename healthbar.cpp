#include "healthbar.h"

Healthbar::Healthbar(int &currHp, int &maxHp) :
    _currHp(currHp), _maxHp(maxHp){}

void Healthbar::draw(int x, int y, QPainter &painter, double mapZoom){

    QPen pen;
    QPen oldPen = pen;
    int healthWidth = 70;
    healthWidth *= mapZoom;
    int healthHeight = 13;
    healthHeight *= mapZoom;
    pen.setWidth(healthHeight);
    painter.setPen(pen);
    painter.drawLine(x, y, x + healthWidth, y);

    pen.setWidth(healthHeight - 2);
    double percent = (double)_currHp / _maxHp;
    if (percent > .6)
        pen.setColor(Qt::green);
    else if (percent > .3)
        pen.setColor(Qt::yellow);
    else
        pen.setColor(Qt::red);

    painter.setPen(pen);
    if (percent > 0)
        painter.drawLine(x + 1,
                         y + 1,
                         x + (healthWidth * percent) - 1,
                         y + 1);

    painter.setPen(oldPen);
}
