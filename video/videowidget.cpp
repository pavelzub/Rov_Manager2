#include "videowidget.hpp"
#include "videostreamparser.hpp"

#include <QApplication>
#include <QMouseEvent>
#include <QThread>
#include <iostream>

VideoWidget::VideoWidget(QWidget *parent):
    QVideoWidget(parent),
    _imageDetector(new ImageDetector(this))
{
    setFixedSize(WIDGETWIDTH, WIDGETWIDTH * HEIGHT / WIDTH);
    _initFfmpeg();
    _initThread();
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
//        Qfont font = setPointSize(10);
//        painter->setFont(font);
        painter.drawText(_imageDetector->getRect(), Qt::AlignTop, FIGURENAMES[_imageDetector->getType() - 1]);
    }

    painter.end();
    return;
}

void VideoWidget::_initThread()
{
    QThread* thread = new QThread;
    VideoStreamParser* parser = new VideoStreamParser(URL);

    parser->moveToThread(thread);

    connect(thread, &QThread::started, parser, &VideoStreamParser::process);
    connect(parser, &VideoStreamParser::finished, thread, &QThread::quit);
    connect(parser, &VideoStreamParser::repaint, this, &VideoWidget::_update);
    connect(parser, &VideoStreamParser::finished, parser, &VideoStreamParser::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();
}

void VideoWidget::_initFfmpeg()
{
    av_register_all();
    avformat_network_init();
    avcodec_register_all();
}

void VideoWidget::_update(QPixmap pixmap)
{
    _pixmap = pixmap;
    if (_searching) _imageDetector->detectImage(_pixmap);
    repaint();
}

