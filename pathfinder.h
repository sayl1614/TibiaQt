#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <QPixmap>
#include <vector>
#include <queue>
#include <QString>

#include <QPainter>
#include <QRect>
#include <QBrush>
#include <QVector>


#include "enums.h"

#include "worldmap.h"
class WorldMap;

class MainWindow;

class PathFinder{
public:
    PathFinder(MainWindow *parent, WorldMap *theMap);
    //PathFinder(WorldMap *theMap, const double &mapZoom, const int &drawTileSize);
    void draw(QPainter &painter);


    WorldMap *_theMap;

    QVector<QPixmap> _highlighted;
    QVector<QPoint> _wayBack; // unused?

    struct Node{
        Node();
        Node(QPoint pos) : _pos(pos){}
        Node(int x, int y){_pos.setX(x); _pos.setY(y);}
        Node(Node *&otherNode){
            this->_g = otherNode->_g;
            this->_h = otherNode->_h;
            this->_f = otherNode->_f;
        }

        QPoint _pos;
        Node *_parentNode = nullptr;
        bool _closed = false;
        bool _foundPath = false;

        bool *_latestVisitor;

        int _g = 0; // Movement cost sofar
        int _h = 0; // Cost from node to finish
        int _f = 0; // Total estimated cost (g + h)

        void operator =(Node &node){
            this->_g = node._g;
            this->_h = node._h;
            this->_f = node._f;
        }

        int manhattanDistance(Node *goalNode, int drawingTileSize){

            int distanceX = (abs(_pos.x() - goalNode->_pos.x()));
            //distanceX += distanceX > 0 ? - 1 : 0;
            int distanceY = (abs(_pos.y() - goalNode->_pos.y()));
            //distanceY += distanceY > 0 ? - 1 : 0;
            //diff !diag
            int diagonal;
            if (distanceX < distanceX)
                diagonal = distanceX;
            else
                diagonal = distanceY;

            int straight = abs(distanceX - distanceY);
        }
        void calculateNodePos(Node *endNode, Node *currNode, int gravity){
            this->_parentNode = currNode;

            int distanceX = (abs(_pos.x() - endNode->_pos.x()));
            distanceX += distanceX > 0 ? - 1 : 0;
            int distanceY = (abs(_pos.y() - endNode->_pos.y()));
            distanceY += distanceY > 0 ? - 1 : 0;

            /*
            // Diagonal calculation of H
            int diagonal;
            if (distanceX < distanceX)
                diagonal = distanceX;
            else
                diagonal = distanceY;

            int straight = abs(distanceX - distanceY);

            int totalGravity = straight + diagonal * 2;
            _h = totalGravity;
            /**/

            int tileSize = 100;
            _h = (distanceX + distanceY) * tileSize;
            _g = gravity  + currNode->_g;
            _f = _g + _h;
        }


    };

    struct Prio{ // Final
    public:
        bool operator() (Node *first, Node *second){
            if (first->_f == second->_f){
                if (first->_closed)
                    return true;
                if (second->_closed)
                    return false;
                return first->_h > second->_h;
            }
            else
                return first->_f > second->_f;
        }
    };


    std::priority_queue<Node*, std::vector<Node*>, Prio> _prioFirst;
    std::priority_queue<Node*, std::vector<Node*>, Prio> _prioEnd;

    QVector<Node*> _finalPath;

    QVector < QVector <Node*> > _grid;

    bool _from = true;
    bool _to = true;

    FacingDirection pathfinderSingleDirectional(QPoint first, QPoint second, bool debugMode = false);

    FacingDirection pathfinderBiDirectional(QPoint first, QPoint second, bool debugMode = false);

    Node *_currNode = nullptr;
    Node *_adjacent = nullptr;

    Node *_startNode = nullptr;
    Node *_endNode = nullptr;

    Node *_finalNode = nullptr;

    MainWindow *_parent;


    FacingDirection getDirection(QPoint &start, QPoint &next);
    FacingDirection pathfinderSingleDirectional(QPoint *begin, QPoint *end, bool debugMode = false);
    FacingDirection pathfinderSingleDirectional(QPoint begin, QPoint *end, bool debugMode = false);
    FacingDirection pathfinderSingleDirectional(QPoint *begin, QPoint end, bool debugMode = false);
    FacingDirection pathfinderBiDirectional(QPoint *begin, QPoint end, bool debugMode = false);
    FacingDirection pathfinderBiDirectional(QPoint begin, QPoint *end, bool debugMode = false);
    FacingDirection pathfinderBiDirectional(QPoint *begin, QPoint *end, bool debugMode = false);
protected:
    void initialStage(Node *first, Node *second, std::priority_queue<Node*, std::vector<Node*>, Prio> &_prio);
    FacingDirection findClosestPath(bool *me, Node *endNode,std::priority_queue<Node*, std::vector<Node*>, Prio> &_prio, bool debugMode = false);


    void addNode(bool *me, Node *_endNode, int gravityMultiplier, int x, int y, std::priority_queue<Node*, std::vector<Node*>, Prio> &_prio);
    void resetGrid();
    void clearMemory();


    int distanceToEnemy(QPoint start, QPoint end);
    int distanceToGoal(QPoint start, QPoint end);
};

#endif // PATHFINDER_H

