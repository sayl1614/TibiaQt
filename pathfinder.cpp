#include "pathfinder.h"

//#include "mainwindow.h"

PathFinder::PathFinder(MainWindow *parent, WorldMap *theMap) :
    _parent(parent), _theMap(theMap){

    _highlighted.push_back(QPixmap(":/images/tiles/green.png"));
    _highlighted.push_back(QPixmap(":/images/tiles/red.png"));
    _highlighted.push_back(QPixmap(":/images/tiles/black.png"));
    _grid.resize(theMap->getMapWidth());
    for(int y = 0; y < theMap->getMapHeight(); y++){
        for(int x = 0; x < theMap->getMapWidth(); x++){
            _grid[x].push_back(nullptr);
        }
    }
}

FacingDirection PathFinder::pathfinderSingleDirectional(QPoint *begin, QPoint *end, bool debugMode){
    return pathfinderSingleDirectional(QPoint(begin->x(), begin->y()),
                                QPoint(end->x(), end->y()), debugMode);
}
FacingDirection PathFinder::pathfinderSingleDirectional(QPoint begin, QPoint *end, bool debugMode){
    return pathfinderSingleDirectional(begin, QPoint(end->x(), end->y()), debugMode);
}
FacingDirection PathFinder::pathfinderSingleDirectional(QPoint *begin, QPoint end, bool debugMode){
    return pathfinderSingleDirectional(QPoint(begin->x(), begin->y()), end, debugMode);
}

FacingDirection PathFinder::pathfinderSingleDirectional(QPoint begin, QPoint end, bool debugMode){
    if (!_startNode){
        if (distanceToEnemy(begin, end) == 0)
            return FacingDirection::notFound;
        _startNode = new Node(begin);
        _endNode = new Node(end);
        initialStage(_startNode, _endNode, _prioFirst);
    }
    return findClosestPath(&_from, _endNode, _prioFirst, debugMode);
}


FacingDirection PathFinder::pathfinderBiDirectional(QPoint *begin, QPoint end, bool debugMode){
    return pathfinderBiDirectional(QPoint(begin->x(), begin->y()), end, debugMode);
}
FacingDirection PathFinder::pathfinderBiDirectional(QPoint begin, QPoint *end, bool debugMode){
    return pathfinderBiDirectional(begin, QPoint(end->x(), end->y()), debugMode);
}
FacingDirection PathFinder::pathfinderBiDirectional(QPoint *begin, QPoint *end, bool debugMode){
    return pathfinderBiDirectional(QPoint(begin->x(), begin->y()),
                            QPoint(end->x(), end->y()), debugMode);
}
FacingDirection PathFinder::pathfinderBiDirectional(QPoint begin, QPoint end, bool debugMode){
    if (!_startNode){
        if (distanceToEnemy(begin, end) == 0)
            return FacingDirection::notFound;
        _startNode = new Node(begin);
        _endNode = new Node(end);
        initialStage(_startNode, _endNode, _prioFirst);
        initialStage(_endNode, _startNode, _prioEnd);
    }
    FacingDirection direction = FacingDirection::continueToNext;
    while (true){
        direction = findClosestPath(&_from, _endNode, _prioFirst, debugMode); // Primary search
        if (direction != FacingDirection::continueToNext)
            return direction;
        if (!_currNode)
            direction = findClosestPath(&_to, _startNode, _prioEnd, debugMode); // Secondary search

        if (debugMode)
            break;
    }
}

void PathFinder::initialStage(Node *startNode, Node *endNode, std::priority_queue<Node*, std::vector<Node*>, Prio> &_prio){
    _currNode = new Node(startNode->_pos);
    double tileGravity = _theMap->getTile(_currNode->_pos)->getTileGarvity();
    _currNode->_g = -tileGravity;

    _currNode->calculateNodePos(endNode, _currNode, tileGravity);
    _prio.push(_currNode);
    _grid[(_currNode->_pos.x())][_currNode->_pos.y()] = _currNode;
}

FacingDirection PathFinder::findClosestPath(bool *me, Node *goalNode,  std::priority_queue<Node*, std::vector<Node*>, Prio> &_prio, bool debugMode){
    while(true){
        if (_prio.empty()){
            if (_childNode && me == &_to){
                return FacingDirection::notFound;
            }
            else{
                clearMemory();
                return FacingDirection::notFound;
            }
        }

        if (_prio.size() > 25){
            int x = 0;
            int y = x;
        }
        if (_prio.size() > 50){
            int x = 0;
            int y = x;
        }
        if (_prio.size() > 100){
            int x = 0;
            int y = x;
        }
        if (_prio.size() > 150){
            int x = 0;
            int y = x;
        }




        while (_prio.size()){
            if (_prio.top()->_closed == true){
                _prio.pop();
                continue;
            }
            _currNode = _prio.top();
            _prio.pop();
            _currNode->_closed = true;
            break;
        }

        int distanceX = (abs(_currNode->_pos.x() - goalNode->_pos.x()));
        int distanceY = (abs(_currNode->_pos.y() - goalNode->_pos.y()));

        distanceX += distanceX > 0 ? - 1 : 0;
        distanceY += distanceY > 0 ? - 1 : 0;

        // Found path!
        if (!(distanceX + distanceY) || _childNode){
            _currNode->_closed = false;
            if (me != &_from)
                return FacingDirection::continueToNext;
            if (_childNode)
                _currNode = _childNode;
            while(_currNode->_parentNode->_parentNode != _currNode->_parentNode){
                _currNode = _currNode->_parentNode;
            }
            FacingDirection direction = getDirection(_currNode->_parentNode->_pos, _currNode->_pos);
            clearMemory();
            return direction;
        }

        int gravity = _theMap->getTile(_currNode->_pos)->getTileGarvity(); // vertical / horozontal
        bool prioVertital = distanceY > distanceX;
        if (prioVertital){
            // North node
            addNode(me, goalNode, gravity, (_currNode->_pos.x()), (_currNode->_pos.y() - 1), _prio);
            if (_childNode){
                continue;
            }
            // South node
            addNode(me, goalNode, gravity, (_currNode->_pos.x()), _currNode->_pos.y() + 1, _prio);
            if (_childNode){
                continue;
            }
            // West node
            addNode(me, goalNode, gravity, _currNode->_pos.x() - 1, _currNode->_pos.y(), _prio);
            if (_childNode){
                continue;
            }
            // East node
            addNode(me, goalNode, gravity, _currNode->_pos.x() + 1, _currNode->_pos.y(), _prio);
            if (_childNode){
                continue;
            }
        }
        else{
            // West node
            addNode(me, goalNode, gravity, _currNode->_pos.x() - 1, _currNode->_pos.y(), _prio);
            if (_childNode){
                continue;
            }
            // East node
            addNode(me, goalNode, gravity, _currNode->_pos.x() + 1, _currNode->_pos.y(), _prio);
            if (_childNode){
                continue;
            }
            // North node
            addNode(me, goalNode, gravity, _currNode->_pos.x(), _currNode->_pos.y() - 1, _prio);
            if (_childNode){
                continue;
            }
            // South node
            addNode(me, goalNode, gravity, _currNode->_pos.x(), _currNode->_pos.y() + 1, _prio);
            if (_childNode){
                continue;
            }
        }
        /*
    gravity = 30; // diagonal
    // North west
    addNode(me, goalNode, gravity,(_currNode->_pos.x() - _drawTileSize) / _drawTileSize, (_currNode->_pos.y() - _drawTileSize) / _drawTileSize, _prio);
    if (_childNode){
        continue;
    }
    // North East
    addNode(me, goalNode, gravity,(_currNode->_pos.x() + _drawTileSize) / _drawTileSize, (_currNode->_pos.y() - _drawTileSize) / _drawTileSize, _prio);
    if (_childNode){
        continue;
    }
    // South west
    addNode(me, goalNode, gravity,(_currNode->_pos.x() - _drawTileSize) / _drawTileSize, (_currNode->_pos.y() + _drawTileSize) / _drawTileSize, _prio);
    if (_childNode){
        continue;
    }
    // South east
    addNode(me, goalNode, gravity,(_currNode->_pos.x() + _drawTileSize) / _drawTileSize, (_currNode->_pos.y() + _drawTileSize) / _drawTileSize, _prio);
    if (_childNode){
        continue;
    }
    */

        _currNode = nullptr;
        return FacingDirection::continueToNext;
    }
}



void PathFinder::addNode(bool *me, Node *goalNode, int gravity, int x, int y, std::priority_queue<Node*, std::vector<Node*>, Prio> &_prio){
    if (!_parent->isWalkable(x, y))
        return;
    _childNode = _grid[x][y];
    if (_childNode == nullptr){
        _childNode = new Node(x, y);
        _childNode->calculateNodePos(goalNode, _currNode, gravity);
        _childNode->_latestVisitor = me;
        _prio.push(_childNode);
        _grid[x][y] = _childNode;
    }
    else if(!_childNode->_closed) {
        if (_childNode->_latestVisitor == me) {
            int oldF = _childNode->_f;
            int oldG = _childNode->_g;
            int oldH = _childNode->_h;
            Node *parent = _childNode->_parentNode;

            _childNode->calculateNodePos(goalNode, _currNode, gravity);
            if (oldF > _childNode->_f){
                _prio.push(_childNode);
                _grid[x][y] = _childNode;
            }
            else{
                _childNode->_f = oldF;
                _childNode->_g = oldG;
                _childNode->_h = oldH;
                _childNode->_parentNode = parent;
            }
        }
        else{ // Found!
            if (me == &_from){
                _childNode->_foundPath = true;
                _childNode->calculateNodePos(goalNode, _currNode, gravity);
            }
            else if (me == &_to){
                _childNode->_foundPath = true;
            }
            return;
        }
    }
    _childNode = nullptr;
}

FacingDirection PathFinder::getDirection(QPoint &start, QPoint &next){
    if (next.x() == start.x()){
        if (next.y() == start.y()){
            return FacingDirection::notFound;
        }
        if (next.y() < start.y()){
            return FacingDirection::north;
        }
        else{
            return FacingDirection::south;
        }
    }
    else if (next.y() == start.y()){
        if (next.x() == start.x())
            return FacingDirection::notFound;
        if (next.x() < start.x()){
            return FacingDirection::west;
        }
        else{
            return FacingDirection::east;
        }
    }
    else{
        assert(1 == 2);
        // Strafe!!! fix!
    }
}

int PathFinder::distanceToEnemy(QPoint start, QPoint end){
    int distanceX = (abs(start.x() - end.x()));
    int distanceY = (abs(start.y() - end.y()));

    distanceX += distanceX > 0 ? - 1 : 0;
    distanceY += distanceY > 0 ? - 1 : 0;

    return distanceX + distanceY;
}

int PathFinder::distanceToGoal(QPoint start, QPoint end){
    int distanceX = (abs(start.x() - end.x()));
    int distanceY = (abs(start.y() - end.y()));
    return distanceX + distanceY;
}

void PathFinder::clearMemory(){
    resetGrid();
    while (_prioFirst.size())
        _prioFirst.pop();
    while (_prioEnd.size())
        _prioEnd.pop();
    _currNode = nullptr;
    _startNode = nullptr;
    _endNode = nullptr;
    _childNode = nullptr;
}

void PathFinder::resetGrid(){
    for(int y = 0; y < _theMap->getMapHeight(); y++){
        for(int x = 0; x < _theMap->getMapWidth(); x++){
            delete _grid[x][y];
            _grid[x][y] = nullptr;
        }
    }
}

void PathFinder::draw(QPainter &painter){

    int xOffsetG = 5;
    int yOffsetG = 20;
    int xOffsetH = 70;
    int yOffsetH = 20;
    int xOffsetF = 40;
    int yOffsetF = 70;

    int indexPosY = _theMap->getIndexPos('y');

    for (int drawYPos = -_parent->dimentions.getDrawTileSize() - _parent->_offset->y(); drawYPos < 900; drawYPos += _parent->dimentions.getDrawTileSize(), indexPosY++){
        if (indexPosY < 0 || indexPosY >= _theMap->getMapHeight())
            continue;

        int indexPosX = _theMap->getIndexPos('x');
        for (int drawXPos = -_parent->dimentions.getDrawTileSize() - _parent->_offset->x(); drawXPos < 1500; drawXPos += _parent->dimentions.getDrawTileSize(), indexPosX++){
            if (indexPosX < 0 || indexPosX >= _theMap->getMapWidth())
                continue;


            if (_grid[indexPosX][indexPosY] == nullptr)
                continue;

            if (_grid[indexPosX][indexPosY]->_foundPath)
                painter.drawPixmap(drawXPos, drawYPos, _parent->dimentions.getDrawTileSize(), _parent->dimentions.getDrawTileSize(), _highlighted[2]);
            else if (_grid[indexPosX][indexPosY]->_closed)
                painter.drawPixmap(drawXPos, drawYPos, _parent->dimentions.getDrawTileSize(), _parent->dimentions.getDrawTileSize(), _highlighted[1]);
            else
                painter.drawPixmap(drawXPos, drawYPos, _parent->dimentions.getDrawTileSize(), _parent->dimentions.getDrawTileSize(), _highlighted[0]);

            if (_parent->dimentions.getDrawTileSize() == 100){
                painter.drawText(drawXPos + xOffsetH, drawYPos + yOffsetH, QString::number(_grid[indexPosX][indexPosY]->_h));
                painter.drawText(drawXPos + xOffsetG, drawYPos + yOffsetG, QString::number(_grid[indexPosX][indexPosY]->_g));
                painter.drawText(drawXPos + xOffsetF, drawYPos + yOffsetF, QString::number(_grid[indexPosX][indexPosY]->_f));
            }
        }
    }
}

