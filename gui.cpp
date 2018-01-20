#include "gui.h"

#include "mainwindow.h"

GUI::GUI(MainWindow *parent, QVector<QVector<Tile*>> &theMap) :
    _parent(parent), _theMap(theMap){

}

int GUI::getIndexPos(char index){
    if (index == 'x')
        return floor((_parent->_screenPos->x()) -
                              (_parent->_screenCenter.x() / _parent->dimentions.getMapZoom()));
    else // if 'y';
        return floor((_parent->_screenPos->y()) -
                              (_parent->_screenCenter.y() / _parent->dimentions.getMapZoom()));
}

void GUI::drawMap(QPainter &painter){

    // Draw tiles (and items?)
    int indexPosY = getIndexPos('y');

    int mapHeight = 100;
    int mapWidth = 100;

    for (int drawYPos = -_parent->dimentions.getDrawTileSize() - _parent->_offset->y(); drawYPos < 850; drawYPos += _parent->dimentions.getDrawTileSize(), indexPosY++){
        if (indexPosY < 0 || indexPosY >= mapHeight)
            continue;

        int indexPosX = getIndexPos('x');
        for (int drawXPos = -_parent->dimentions.getDrawTileSize() - _parent->_offset->x(); drawXPos < 1500; drawXPos += _parent->dimentions.getDrawTileSize(), indexPosX++){
            if (indexPosX < 0 || indexPosX >= mapWidth)
                continue;


            _theMap[indexPosX][indexPosY]->drawTile( drawXPos, drawYPos, _parent->dimentions.getMapZoom(), painter);
        }
    }

    // Draw characters
    indexPosY = getIndexPos('y');

    for (int drawYPos = -_parent->dimentions.getDrawTileSize() - _parent->_offset->y(); drawYPos < 850; drawYPos += _parent->dimentions.getDrawTileSize(), indexPosY++){
        if (indexPosY < 0 || indexPosY >= mapHeight)
            continue;

        int indexPosX = getIndexPos('x');

        for (int drawXPos = -_parent->dimentions.getDrawTileSize() - _parent->_offset->x(); drawXPos < 1500; drawXPos += _parent->dimentions.getDrawTileSize(), indexPosX++){
            if (indexPosX < 0 || indexPosX >= mapWidth)
                continue;

            _theMap[indexPosX][indexPosY]->drawCharacters(drawXPos, drawYPos, painter);
        }
    }
}
