#ifndef GUI_H
#define GUI_H


class MainWindow;
#include "worldmap.h"

#include <QVector>


class GUI{
public:
    GUI(MainWindow *_parent, QVector<QVector<Tile*>> &worldmap);

    void drawMap(QPainter &painter);
    int getIndexPos(char index);
private:
    QVector<QVector<Tile*>> &_theMap;
    MainWindow *_parent;
};

#endif // GUI_H
