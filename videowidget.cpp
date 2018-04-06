#include "videowidget.hpp"
#include "videostreamparser.hpp"

#include <QApplication>
#include <QMouseEvent>
#include <QThread>
#include <iostream>

VideoWidget::VideoWidget(QWidget *parent):
    QVideoWidget(parent)
{
    setFixedSize(WIDGETWIDTH, WIDGETWIDTH * HEIGHT / WIDTH);
    _initFfmpeg();
    _initThread();
}

QPixmap VideoWidget::GetPixmap()
{
    return _pixmap;
}

void VideoWidget::paintEvent(QPaintEvent *event)
{
    QVideoWidget::paintEvent(event);

    if (_pixmap.size() != QSize(0, 0)){
        QPainter painter(this);
        painter.drawPixmap(rect(), _pixmap);
        painter.end();
    }

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
    repaint();
}

