#include "image.h"

Image::Image(QString path){
    _playInterval = new QTimer(this);
    connect(_playInterval, SIGNAL(timeout()), this, SLOT(nextImage()));
    loadImage(path);
}

void Image::init(){

}

void Image::draw(int x, int y, double zoom, QPainter &painter){
    double drawSize = zoom * _imageSize;
    painter.drawPixmap(x, y, drawSize, drawSize, _image[_currentImage]);
}

void Image::loadImage(QString path){
    QPixmap image;
    int counter = 1;
    while (image.load(path + "/" + QString::number(counter++) + ".png")){
        _image.push_back(image);
    }
    _imageSize = _image.front().width();
}

void Image::play(){
    _playInterval->start(this->_animationInterval);
}

void Image::play(int msPerSquare){
    _playInterval->start(msPerSquare / _framesPerSquare);
}

void Image::stop(){
    _playInterval->stop();
}

bool Image::isPlaying(){
    return _playInterval->isActive();
}

void Image::reset(){
    this->_currentImage = 0;
}

void Image::reloadImage(QString path){
    while (_image.size())
        _image.pop_back();
    loadImage(path);
}

int Image::getWidth(){
    return _image[0].width();
}

void Image::nextImage(){
    if (++_currentImage >= _image.size())
        _currentImage = _image.size() > 2 ? 1 : 0;
}
