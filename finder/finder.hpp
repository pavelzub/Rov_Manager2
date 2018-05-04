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

    bool _detectFigure(QPixmap pixmap);
    bool _detectText(QPixmap pixmap);
    FigureColor _getFigureColor(QColor color);
    float _getSquare(std::vector<cv::Point2f> poitns);
    QRect _getRect(std::vector<cv::Point2f> poitns);
    cv::Mat _getGrauScaleMat(QImage image);

    Type* _type;
    QRect* _rect;
    QSettings* _settings;
};

#endif // FINDER_HPP
