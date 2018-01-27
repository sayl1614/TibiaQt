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
    FacingDirection direction;
    while (true){
        direction = findClosestPath(&_from, _endNode, _prioFirst, debugMode); // Primary search
        if (direction != FacingDirection::continueToNext)
            return direction;
        if (findClosestPath(&_to, _startNode, _prioEnd, debugMode) == FacingDirection::notFound) // Secondary search
            return FacingDirection::notFound;
        if (debugMode)
            break;
    }
}

void PathFinder::initialStage(Node *startNode, Node *endNode, std::priority_queue<Node*, std::vector<Node*>, Prio> &_prio){
    int tileSize = 100;
    _currNode = new Node(startNode->_pos);
    double tileGravity = _theMap->getTile(_currNode->_pos)->getTileGarvity();
    _currNode->_g = tileSize;

    _currNode->calculateNodePos(endNode, _currNode, tileGravity);
    _adjacent = _currNode;
    _prio.push(_currNode);
    _grid[(_currNode->_pos.x())][_currNode->_pos.y()] = _currNode;
}

FacingDirection PathFinder::findClosestPath(bool *me, Node *goalNode,  std::priority_queue<Node*, std::vector<Node*>, Prio> &_prio, bool debugMode){
    while(true){
        if (_prio.empty()){
            if (_currNode->_foundPath){
                if (me == &_to){
                    return FacingDirection::continueToNext;
                }
            }
            else{
                clearMemory();
                return FacingDirection::notFound;
            }
        }
        while (_prio.size()){
            if (_currNode->_foundPath)
                break;
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
        if (!(distanceX + distanceY) || _currNode->_foundPath){
            if (me == &_to)
                return FacingDirection::continueToNext;
            while(_currNode->_parentNode->_parentNode != _currNode->_parentNode){
                _currNode = _currNode->_parentNode;
            }
            FacingDirection direction = getDirection(_currNode->_parentNode->_pos, _currNode->_pos);
            clearMemory();
            return direction;
        }
        int gravityMultiplier = 1;
        bool prioVertital = distanceY > distanceX;
        if (prioVertital){
            // North node
            addNode(me, goalNode, gravityMultiplier, (_currNode->_pos.x()), (_currNode->_pos.y() - 1), _prio);
            if (_adjacent->_foundPath){
                _currNode = _adjacent;
                continue;
            }
            // South node
            addNode(me, goalNode, gravityMultiplier, (_currNode->_pos.x()), _currNode->_pos.y() + 1, _prio);
            if (_adjacent->_foundPath){
                _currNode = _adjacent;
                continue;
            }
            // West node
            addNode(me, goalNode, gravityMultiplier, _currNode->_pos.x() - 1, _currNode->_pos.y(), _prio);
            if (_adjacent->_foundPath){
                _currNode = _adjacent;
                continue;
            }
            // East node
            addNode(me, goalNode, gravityMultiplier, _currNode->_pos.x() + 1, _currNode->_pos.y(), _prio);
            if (_adjacent->_foundPath){
                _currNode = _adjacent;
                continue;
            }
        }
        else{
            // West node
            addNode(me, goalNode, gravityMultiplier, _currNode->_pos.x() - 1, _currNode->_pos.y(), _prio);
            if (_adjacent->_foundPath){
                _currNode = _adjacent;
                continue;
            }
            // East node
            addNode(me, goalNode, gravityMultiplier, _currNode->_pos.x() + 1, _currNode->_pos.y(), _prio);
            if (_adjacent->_foundPath){
                _currNode = _adjacent;
                continue;
            }
            // North node
            addNode(me, goalNode, gravityMultiplier, _currNode->_pos.x(), _currNode->_pos.y() - 1, _prio);
            if (_adjacent->_foundPath){
                _currNode = _adjacent;
                continue;
            }
            // South node
            addNode(me, goalNode, gravityMultiplier, _currNode->_pos.x(), _currNode->_pos.y() + 1, _prio);
            if (_adjacent->_foundPath){
                _currNode = _adjacent;
                continue;
            }
        }
        /*
        gravityMultiplier = 2;
        // North west
        addNode(me, goalNode, gravityMultiplier, _currNode->_pos.x() - 1, _currNode->_pos.y() - 1, _prio);
        if (_adjacent->_foundPath){
            _currNode = _adjacent;
            continue;
        }
        // North East
        addNode(me, goalNode, gravityMultiplier, _currNode->_pos.x() + 1, _currNode->_pos.y() - 1, _prio);
        if (_adjacent->_foundPath){
            _currNode = _adjacent;
            continue;
        }
        // South west
        addNode(me, goalNode, gravityMultiplier, _currNode->_pos.x() - 1, _currNode->_pos.y() + 1, _prio);
        if (_adjacent->_foundPath){
            _currNode = _adjacent;
            continue;
        }
        // South east
        addNode(me, goalNode, gravityMultiplier, _currNode->_pos.x() + 1, _currNode->_pos.y() + 1, _prio);
        if (_adjacent->_foundPath){
            _currNode = _adjacent;
            continue;
        }
        */
        return FacingDirection::continueToNext;
    }
}



void PathFinder::addNode(bool *me, Node *goalNode, int gravityMultiplier, int x, int y, std::priority_queue<Node*, std::vector<Node*>, Prio> &_prio){
    if (!_theMap->isWalkable(x, y))
        return;
    _adjacent = _grid[x][y];
    int gravity = _theMap->getTile(QPoint(x, y))->getTileGarvity() * gravityMultiplier;
    if (_adjacent == nullptr){
        _adjacent = new Node(x, y);
        _adjacent->calculateNodePos(goalNode, _currNode, gravity);
        _adjacent->_latestVisitor = me;
        _prio.push(_adjacent);
        _grid[x][y] = _adjacent;
    }
    else if(!_adjacent->_closed) {
        if (_adjacent->_latestVisitor == me) {
            int oldF = _adjacent->_f;
            int oldG = _adjacent->_g;
            int oldH = _adjacent->_h;
            Node *parent = _adjacent->_parentNode;

            _adjacent->calculateNodePos(goalNode, _currNode, gravity);
            if (oldF > _adjacent->_f){
                _prio.push(_adjacent);
                _grid[x][y] = _adjacent;
            }
            else{
                _adjacent->_f = oldF;
                _adjacent->_g = oldG;
                _adjacent->_h = oldH;
                _adjacent->_parentNode = parent;
            }
        }
        else{ // Found!
            if (me == &_from){
                _adjacent->_foundPath = true;
                _adjacent->calculateNodePos(goalNode, _currNode, gravity);
            }
            else if (me == &_to){
                _adjacent->_foundPath = true;
            }
            return;
        }
    }
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
        //assert(1 == 2);
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
    _adjacent = nullptr;
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

