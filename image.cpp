#include "image.h"

Image::Image(QString image){
    _playInterval = new QTimer(this);
    connect(_playInterval, SIGNAL(timeout()), this, SLOT(nextImage()));
    loadImage(image);
}

void Image::draw(int x, int y, QPainter &painter, double zoom){
    zoom = zoom * _imageSize;
    painter.drawPixmap(x, y, zoom, zoom, _image[_currentImage]);
}

void Image::loadImage(QString path){
    QPixmap image;
    int counter = 1;
    while (image.load(path + "/" + QString::number(counter++) + ".png")){
        _image.push_back(image);
    }
    _imageSize = _image.front().width();
}

void Image::stop(){
    _playInterval->stop();
}

bool Image::isPlaying(){
    return _playInterval->isActive();
}

void Image::play(int interval){
    _playInterval->start(interval);
}

void Image::reset(){
    this->_currentImage = 0;
}

void Image::reloadImage(QString path){
    while (_image.size())
        _image.pop_back();
    loadImage(image);
}

void Image::nextImage(){
    if (++_currentImage >= _image.size())
        _currentImage = 1;
}

Image::~Image(){
    delete _playInterval;
}
