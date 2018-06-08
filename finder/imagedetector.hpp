#ifndef IMAGEDETECTOR_HPP
#define IMAGEDETECTOR_HPP

#include <QObject>
#include <QBitmap>

#include "settings.hpp"

enum FigureType {NONE, REDTRIANGLE, YELLOWTRIANGLE, BLUETRIANGLE, REDRECT, YELLOWRECT, BLUERECT};
enum FigureColor {OTHER, RED, YELLOW, BLUE};
const QString FIGURENAMES[6] = {"A", "B", "C",
                                "D", "E", "F"};
const QString TEMPLATESPATH[6] = {"1.jpg", "2.jpg", "3.jpg",
                                   "4.jpg", "5.jpg", "6.jpg"};

class ImageDetector : public QObject
{
    Q_OBJECT
public:
    explicit ImageDetector(Settings* settings, QObject *parent = nullptr);
    void detectImage(QPixmap pixmap);
    bool figureIsFound();
    bool isWorking();
    QRect getRect();
    FigureType getType();
    void UpdateSettings();

signals:
    void detect(QPixmap);

private slots:
    void _stopDetection(FigureType type, QRect rect);

private:
    void _createThread();

    bool _isWorking = false;
    FigureType _type = NONE;
    QRect _rect;
    Settings* _settings;
};

#endif // IMAGEDETECTOR_HPP
