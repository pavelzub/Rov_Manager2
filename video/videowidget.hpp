#ifndef MYVIDEOWIDGET_HPP
#define MYVIDEOWIDGET_HPP

#include <QWidget>
#include <QVideoWidget>
#include <QPainter>
#include <QString>

#include "finder/imagedetector.hpp"

class VideoWidget : public QVideoWidget
{
    Q_OBJECT
public:
    VideoWidget(QWidget *parent = nullptr);
    void StartStopRecognition(bool f);
    void Update(QPixmap pixmap);
    QPixmap GetPixmap();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void _initFfmpeg();

    const int WIDTH = 16;
    const int HEIGHT = 9;
    const int WIDGETWIDTH = 700;
    bool _searching = false;
    ImageDetector* _imageDetector;
    QPixmap _pixmap;

};

#endif // MYVIDEOWIDGET_HPP
