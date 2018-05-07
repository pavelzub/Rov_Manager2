#ifndef IMAGEDETECTOR_HPP
#define IMAGEDETECTOR_HPP

#include <QObject>
#include <QBitmap>
#include <QSettings>

enum Type {NONE, REDTRIANGLE, YELLOWTRIANGLE, BLUETRIANGLE, REDRECT, YELLOWRECT, BLUERECT};
enum FigureColor {OTHER, RED, YELLOW, BLUE};
const QString FIGURENAMES[6] = {"A", "B", "C",
                                "D", "E", "F"};
const QString TEMPLATESPATH[6] = {"1.jpg", "2.jpg", "3.jpg",
                                   "4.jpg", "5.jpg", "6.jpg"};

class ImageDetector : public QObject
{
    Q_OBJECT
public:
    explicit ImageDetector(QSettings* settings, QObject *parent = nullptr);
    void detectImage(QPixmap pixmap);
    bool figureIsFound();
    bool isWorking();
    QRect getRect();
    Type getType();

signals:
    void detect(QPixmap);

private slots:
    void _stopDetection();

private:
    void _createThread();

    bool _isWorking = false;
    Type _type = NONE;
    Type _newType;
    QRect _rect;
    QRect _newRect;
    QSettings* _settings;
};

#endif // IMAGEDETECTOR_HPP
