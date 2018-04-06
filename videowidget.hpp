#ifndef MYVIDEOWIDGET_HPP
#define MYVIDEOWIDGET_HPP

#include <QWidget>
#include <QVideoWidget>
#include <QPainter>
#include <QString>

class VideoWidget : public QVideoWidget
{
    Q_OBJECT
public:
    VideoWidget(QWidget *parent = nullptr);
    QPixmap GetPixmap();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void _initThread();
    void _initFfmpeg();

    const int WIDTH = 16;
    const int HEIGHT = 9;
    const int WIDGETWIDTH = 700;
    const QString URL = "udp://192.168.1.255:1234";
    QPixmap _pixmap;

    void _update(QPixmap pixmap);
};

#endif // MYVIDEOWIDGET_HPP
