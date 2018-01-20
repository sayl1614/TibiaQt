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
    Image(QString path);
    void init();

    void draw(int x, int y, double zoom, QPainter &painter);

    void play();
    void play(int speed);
    void stop();
    bool isPlaying();
    void reset();
    void reloadImage(QString path);

    int getWidth();

    ~Image();
    void loadImage(QString item);
private:
    // This attribute is per-object sensitive
    int _animationInterval;

    QVector<QPixmap> _image;
    int _imageSize;

    QTimer *_playInterval;
    int _framesPerSquare = 8;
    int _currentImage = 0;

private slots:
    void nextImage();
};

#endif // IMAGE_H
