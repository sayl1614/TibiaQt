#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "player.h"
#include "npc.h"
#include "worldmap.h"
class WorldMap;

#include "gui.h"
class GUI;

#include "pathfinder.h"
class PathFinder;


#include "enums.h"

#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <QElapsedTimer>

// Draw
#include <QPaintEvent>

// Key events
#include <QKeyEvent>

// Mouse events
#include <QMouseEvent>
#include <QWheelEvent>


#include <QDebug>

#include "movement.h"
class Movement;
#include "image.h"


class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *e);

    void mousePressEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);


    struct Dimentions{
        int getOrgTileSize(){
            return _orgTileSize;
        }
        int getDrawTileSize(){
            return _drawTileSize;
        }
        void setDrawTileSize(int size){
            _drawTileSize = size;
        }
        double getMapZoom(){
            return _mapZoom;
        }
        void setMapZoom(double zoom){
            _mapZoom = zoom;
            _drawTileSize = _orgTileSize * _mapZoom;
        }
    private:
        int _orgTileSize = 100;
        int _drawTileSize;
        double _mapZoom = 1;
    } dimentions;

    /*
    struct Movement{
        Movement() {
            _screenCenter = QPoint(8, 4);
            _screenPos = new QPoint(8, 5);
            _offset = new QPoint(0, 0);
        }
        QPoint getScreenCenter(){
            return _screenCenter;
        }
        QPoint *getScreenPos(){
            return _screenPos;
        }
        void setScreenPos(QPoint pos){
            setScreenPos(pos.x(), pos.y());
        }
        void setScreenPos(QPoint *pos){
            setScreenPos(pos->x(), pos->y());
        }
        void setScreenPos(int x, int y){
            _screenPos->setX(x);
            _screenPos->setY(y);
        }
        void followCharacterWithCamera(Character *character){
            //_screenPos = character->getPos();
            //_offset = character->movement.getOffset();
        }

        QPoint *getOffset(){
            return _offset;
        }
        int getOffsetX(){
            return _offset->x();
        }
        int getOffsetY(){
            return _offset->y();
        }
        void changeOffset(int x, int y){
            _offset->setX(x);
            _offset->setY(y);
        }

    private:
        QPoint _screenCenter;
        QPoint *_screenPos;
        QPoint *_offset;
    } movement;
    */
    QPoint _screenCenter {7, 5};
    QPoint *_screenPos;
    QPoint *_offset;

    void followWithCamera(Character *character){
        auto info = character->getPosReference();
        _screenPos = std::get<0>(info);
        _offset = std::get<1>(info);
    }

    Character *getPlayer(){return _player;}
    WorldMap *getWorldMap(){return _theMap;}
    PathFinder *getPathfinder(){return _pathfinder;}
    int getSpeedPerTileForCharacter(Character *character);
    void addCharacter(Character *newChar);
    void removeCharacter(Character *obj);

    FacingDirection findPath(QPoint startPos,  QPoint endPos);
    bool valid(QPoint &pos);
    bool valid(QPoint *pos);
    bool valid(int x, int y);

private:
    QTimer *_FPSTimer;
    QElapsedTimer _keyPressElapsedTimer;

    WorldMap *_theMap;
    GUI *_GUI;

    Character *_player;
    Character *_enemy;


    // Pathfinder stuff
    PathFinder *_pathfinder;
    bool _ctrlPressed = false;
    bool _altPressed = false;


};

#endif // MAINWINDOW_H
