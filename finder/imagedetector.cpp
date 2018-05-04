#include "imagedetector.hpp"
#include "finder.hpp"
#include <vector>
#include <iostream>
#include <QThread>

ImageDetector::ImageDetector(QSettings *settings, QObject *parent) : QObject(parent)
{
    _settings = settings;
    _createThread();
}

void ImageDetector::detectImage(QPixmap pixmap)
{
    if (_isWorking) return;

    _isWorking = true;

    emit detect(pixmap);
}

bool ImageDetector::figureIsFound()
{
    return _type != NONE;
}

bool ImageDetector::isWorking()
{
    return _isWorking;
}

QRect ImageDetector::getRect()
{
    return _rect;
}

Type ImageDetector::getType()
{
    return _type;
}

void ImageDetector::_stopDetection()
{
    _isWorking = false;
    _type = _newType;
    _rect = _newRect;
}

void ImageDetector::_createThread()
{
    QThread* thread = new QThread;
    Finder* finder = new Finder(_settings, &_newType, &_newRect);

    finder->moveToThread(thread);
    connect(finder, &Finder::findImage, this, &ImageDetector::_stopDetection);
    connect(this, &ImageDetector::detect, finder, &Finder::detect);
    thread->start();
}
