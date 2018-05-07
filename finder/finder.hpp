#ifndef FINDER_HPP
#define FINDER_HPP

#include <QObject>
#include <QSettings>

#include "imagedetector.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/opencv.hpp"

class Finder : public QObject
{
    Q_OBJECT
public:
    explicit Finder(QSettings* settings, Type* type, QRect* rect, QObject *parent = nullptr);

signals:
    void findImage();

public slots:
    void detect(QPixmap pixmap);

private:
    const int COLORTOLERANCE = 100;
    const int MAXSQUARE = 1000000;
    const int MINSQUARE = 10000;
    const QString names[3] = {"RED", "BLUE", "YELLOW"};

    struct Color{
        int hMax, hMin, sMax, sMin, vMax, vMin;
    };
    bool _detectFigure(QPixmap pixmap);
    bool _detectText(QPixmap pixmap);
    cv::Mat _getMask(QPixmap pixmap, int index);
    void _loadSettings();
    FigureColor _getFigureColor(QColor color);
    float _getSquare(std::vector<cv::Point2f> poitns);
    QRect _getRect(std::vector<cv::Point2f> poitns);
    cv::Mat _getGrauScaleMat(QImage image);

    Type* _type;
    QRect* _rect;
    QSettings* _settings;
    Color _colors[3];
};

#endif // FINDER_HPP
