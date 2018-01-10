#ifndef IMAGE_H
#define IMAGE_H

#include "enums.h"

#include <QPixmap>
#include <QTimer>
#include <QString>

#include <QPainter>

class Image : public QObject{
    Q_OBJECT
public:
    Image(QString item);

    void draw(int x, int y, double zoom, QPainter &painter);

    void play();
    void play(int interval);
    void stop();
    bool isPlaying();
    void reset();
    void reloadImage(QString path);

    int getWidth();

    ~Image();
    void loadImage(QString item);
private:
    QVector<QPixmap> _image;
    int _imageSize;

    QTimer *_playInterval;
    int _currentImage = 0;

    int _animationInterval;


private slots:
    void nextImage();
};

#endif // IMAGE_H
