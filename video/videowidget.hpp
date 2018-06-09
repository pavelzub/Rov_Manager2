#ifndef MYVIDEOWIDGET_HPP
#define MYVIDEOWIDGET_HPP

#include <QWidget>
#include <QVideoWidget>
#include <QPainter>
#include <QString>

#include "finder/imagedetector.hpp"
#include "settings.hpp"

class VideoWidget : public QVideoWidget
{
    Q_OBJECT
public:
    VideoWidget(Settings* settings, QWidget *parent = nullptr);
    void StartStopRecognition(bool f);
    void Update(QPixmap pixmap);
    QPixmap GetPixmap();

signals:
    void findFigure(QString s);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    const int WIDTH = 4;
    const int HEIGHT = 3;
    const int WIDGETWIDTH = 700;
    bool _searching = false;
    ImageDetector* _imageDetector;
    QPixmap _pixmap;

};

#endif // MYVIDEOWIDGET_HPP
