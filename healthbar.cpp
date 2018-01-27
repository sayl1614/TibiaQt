#include "healthbar.h"

Healthbar::Healthbar(int &currHp, int &maxHp) :
    _currHp(currHp), _maxHp(maxHp){

}

void Healthbar::draw(int x, int y, QPainter &painter, double mapZoom){

    QPen pen;
    QPen oldPen = pen;
    pen.setWidth(13 * mapZoom);
    painter.setPen(pen);
    painter.drawLine(x, y, x + 100 * mapZoom, y);

    pen.setWidth(11 * mapZoom);
    int percent = ((double)_currHp / _maxHp) * 100;
    if (percent > 60)
        pen.setColor(Qt::green);
    else if (percent > 30)
        pen.setColor(Qt::yellow);
    else
        pen.setColor(Qt::red);

    painter.setPen(pen);
    if (percent > 0)
        painter.drawLine(x + 1,
                         y + 1,
                         x + 1 + (percent * mapZoom),
                         y + 1);

    painter.setPen(oldPen);
}
