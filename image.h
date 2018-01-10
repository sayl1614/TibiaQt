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

    void draw(int x, int y, QPainter &painter, double zoom);

    void play(int interval);
    void stop();
    bool isPlaying();
    void reset();
    void reloadImage(QString path);


    ~Image();
    void loadImage(QString item);
private:
    QVector<QPixmap> _image;
    int _imageSize;

    QTimer *_playInterval;
    int _currentImage = 0;


private slots:
    void nextImage();
};

#endif // IMAGE_H
