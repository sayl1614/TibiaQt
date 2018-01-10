#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    myImage = new Image(":/images/characters/demon/east");
    myImage->play(100);

    dimentions.setDrawTileSize(dimentions.getOrgTileSize()* dimentions.getMapZoom());
    _player = new Player("demon", this);
    _enemy = new Enemy("demon", this);
    _enemy->movement.changePos(25, 25);
    _theMap = new WorldMap(this);
    this->_pathfinder = new PathFinder(this, _theMap);



    this->_FPSTimer = new QTimer(this);
    connect(this->_FPSTimer, SIGNAL(timeout()), this, SLOT(update()));
    this->_FPSTimer->start(1000/60);
    this->_keyPressElapsedTimer.start();
    this->movement.followCharacterWithCamera(_player);

    _enemy->movement.changePos(7, 7);
    _theMap->addCharacter(_enemy);
    for (int i = 0; i < 0; i++)
        _theMap->addCharacter(new Enemy("demon", this, _player, 50));
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if (!_player->isMoving()){
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
        }
    }
    switch(event->key()){
    case Qt::Key_F:{
            _enemy->attack(_player);
            if (myImage->isPlaying())
                myImage->stop();
            else
                myImage->play(100);
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
        _pathfinder->pathfinderSingleDirectional(_player->getPos(), QPoint(5, 5), true);
        break;
    case Qt::Key_B:
        _pathfinder->pathfinderBiDirectional(_player->getPos(), QPoint(5, 5), true);
        break;
    case Qt::Key_M:{
        FacingDirection direction = _pathfinder->pathfinderBiDirectional(_player->getPos(), QPoint(20, 20));
        if (direction != FacingDirection::notFound && !_player->isMoving())
            _player->move(direction);
        break;
    }
    case Qt::Key_Control:
        _ctrlPressed = true;
        break;
    case Qt::Key_Alt:
        _altPressed = true;
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_Control:
        _ctrlPressed = false;
        break;
    case Qt::Key_Alt:
        _altPressed = false;
        break;
    }
}


void MainWindow::mousePressEvent(QMouseEvent *e){
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
}

void MainWindow::wheelEvent(QWheelEvent *event){

}

int MainWindow::getSpeedPerTileForCharacter(Character *character){
    return _theMap->getSpeedPerTileForCharacter(character);
}
int MainWindow::getSpeedPerTileForCharacter(QPoint pos, Character *character){
    return _theMap->getSpeedPerTileForCharacter(pos, character);
}

void MainWindow::addCharacter(Character *obj){
    _theMap->addCharacter(obj);
}
void MainWindow::addCharacter(QPoint *pos, Character *obj){
    _theMap->addCharacter(pos->x(), pos->y(), obj);
}
void MainWindow::removeCharacter(Character *obj){
    _theMap->removeCharacter(obj);
}
void MainWindow::removeCharacter(QPoint *pos, Character *obj){
    _theMap->removeCharacter(pos->x(), pos->y(), obj);
}


FacingDirection MainWindow::findPath(QPoint *startPos,  QPoint *endPos){
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

bool MainWindow::isWalkable(QPoint pos){
    return isWalkable(pos.x(), pos.y());
}

bool MainWindow::isWalkable(int x, int y){
    if (!_theMap->valid(x, y))
        return false;
    if (!_theMap->getTile(x, y)->isWalkable())
        return false;
    if (_theMap->getTile(x, y)->isBusy())
        return false;
    QQueue<Character*> &character = _theMap->getCharacters(x, y);

    for (int i = 0; i < character.size(); i++){
        if (*character[i]->getPos() == QPoint(x, y)){
            return false;
        }
    }
    return true;
}

void MainWindow::paintEvent(QPaintEvent *e){

    QPainter painter(this);

    _theMap->drawMap(painter);
    //_pathfinder->draw(painter);
    myImage->draw(100, 100, dimentions.getMapZoom(),  painter);
}


MainWindow::~MainWindow(){

}
