#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    dimentions.setDrawTileSize(dimentions.getOrgTileSize()* dimentions.getMapZoom());

    this->_FPSTimer = new QTimer(this);
    connect(this->_FPSTimer, SIGNAL(timeout()), this, SLOT(update()));
    this->_FPSTimer->start(1000/60);
    this->_keyPressElapsedTimer.start();


    _player = new Player("demon", this);
    _screenPos = &_screenCenter;
    followWithCamera(_player);

    _theMap = new WorldMap(this);
    this->_pathfinder = new PathFinder(this, _theMap);
    _GUI = new GUI(this, _theMap->getMap());

    _enemy = new NPC("demon", this, nullptr, 150);
    _theMap->addCharacter(_enemy);

    for (int i = 0; i < 2; i++){
        Character *anEnemy = new NPC("demon", this, _player);
        anEnemy->follow(_player);
        _theMap->addCharacter(anEnemy);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
        case Qt::Key_Control:
            _ctrlPressed = true;
            break;
        case Qt::Key_Alt:
            _altPressed = true;
            break;
    }

    switch(event->key()){
    case Qt::Key_Up:
        //_theMap->move(FacingDirection::north);
        _player->move(FacingDirection::north);
        break;
    case Qt::Key_Left:
        _player->move(FacingDirection::west);
        break;
    case Qt::Key_Down:
        _player->move(FacingDirection::south);
        break;
    case Qt::Key_Right:
        _player->move(FacingDirection::east);
        break;
    case Qt::Key_F:{
            _enemy->follow(_player);
            break;
        }
    case Qt::Key_Plus:
        if (dimentions.getMapZoom() < 4.5){
            if (dimentions.getMapZoom() >= 1)
                dimentions.setMapZoom(dimentions.getMapZoom() + 0.1);
            else
                dimentions.setMapZoom(dimentions.getMapZoom() + 0.05);
        }
        break;
    case Qt::Key_Minus:
        if (dimentions.getMapZoom() > .2){
            if (dimentions.getMapZoom() < 1)
                dimentions.setMapZoom(dimentions.getMapZoom() - 0.05);
            else
                dimentions.setMapZoom(dimentions.getMapZoom() - 0.1);
        }
        break;
    case Qt::Key_P:
        _pathfinder->pathfinderSingleDirectional(_player->getEnd(), _enemy->getEnd(), true);
        break;
    case Qt::Key_B:
        _pathfinder->pathfinderBiDirectional(_player->getEnd(), _enemy->getEnd(), true);
        break;
    case Qt::Key_M:{
        FacingDirection direction = _pathfinder->pathfinderBiDirectional(_player->getEnd(),
                                                                         _enemy->getEnd());
        if (direction != FacingDirection::notFound && !_player->isMoving())
            _player->move(direction);
        break;
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
    switch(event->key()){
        case Qt::Key_Control:
            _ctrlPressed = false;
            break;
        case Qt::Key_Alt:
            _altPressed = false;
            break;
    }
}


void MainWindow::mousePressEvent(QMouseEvent *e){
    /*
    int x = e->x() / dimentions.getDrawTileSize();
    int y = e->y() / dimentions.getDrawTileSize();
    int xi = floor((movement.getScreenPos()->x()) + 1 -
                          (movement.getScreenCenter().x() / dimentions.getMapZoom()));

    int yi = floor((movement.getScreenPos()->y()) + 1 -
                          (movement.getScreenCenter().y() / dimentions.getMapZoom()));
    int drawX = x + xi;
    int drawY = y + yi;
    switch (e->button()){
    case Qt::LeftButton:
        if (_ctrlPressed);
            //_player->setPos(QPoint(x, y));
        else if (_altPressed);
            //_enemy->setPos(QPoint(x, y));
        else;
            _theMap->toggleTile(drawX, drawY);
    }
    */
}

void MainWindow::wheelEvent(QWheelEvent *event){

}

int MainWindow::getSpeedPerTileForCharacter(Character *character){
    return _theMap->getSpeedPerTileForCharacter(character);
}

void MainWindow::addCharacter(Character *obj){
    _theMap->addCharacter(obj);
}
void MainWindow::removeCharacter(Character *obj){
    _theMap->removeCharacter(obj);
}

FacingDirection MainWindow::findPath(QPoint startPos,  QPoint endPos){
    return _pathfinder->pathfinderBiDirectional(startPos, endPos);
}

bool MainWindow::valid(QPoint &pos){
    return valid(pos.x(), pos.y());
}
bool MainWindow::valid(QPoint *pos){
    return valid(pos->x(), pos->y());
}
bool MainWindow::valid(int x, int y){
    return _theMap->valid(x, y);
}

void MainWindow::paintEvent(QPaintEvent *e){

    QPainter painter(this);

    _theMap->drawMap(painter);
    //_GUI->drawMap(painter);
    _pathfinder->draw(painter);
}


MainWindow::~MainWindow(){

}
