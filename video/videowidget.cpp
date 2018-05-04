#include "videowidget.hpp"
#include "videostreamparser.hpp"

#include <QApplication>
#include <QMouseEvent>
#include <QThread>
#include <iostream>

VideoWidget::VideoWidget(QSettings *settings, QWidget *parent):
    QVideoWidget(parent),
    _imageDetector(new ImageDetector(settings, this))
{
    setFixedSize(WIDGETWIDTH, WIDGETWIDTH * HEIGHT / WIDTH);
    _initFfmpeg();
}

void VideoWidget::StartStopRecognition(bool f)
{
    _searching = f;
}

QPixmap VideoWidget::GetPixmap()
{
    return _pixmap;
}

void VideoWidget::paintEvent(QPaintEvent *event)
{
    QVideoWidget::paintEvent(event);

    QPainter painter(this);
    if (_pixmap.size() != QSize(0, 0)){
        painter.drawPixmap(rect(), _pixmap);
    }

    if (_imageDetector->figureIsFound())
    {
        painter.drawRect(_imageDetector->getRect());
        painter.drawText(_imageDetector->getRect(), Qt::AlignTop, FIGURENAMES[_imageDetector->getType() - 1]);
    }

    painter.end();
    return;
}

void VideoWidget::_initFfmpeg()
{
    av_register_all();
    avformat_network_init();
    avcodec_register_all();
}

void VideoWidget::Update(QPixmap pixmap)
{
    _pixmap = pixmap;
    if (_searching) _imageDetector->detectImage(_pixmap);
    repaint();
}

