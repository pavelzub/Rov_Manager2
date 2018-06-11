#ifndef FINDER_HPP
#define FINDER_HPP

#include <QObject>

#include "settings.hpp"
#include "imagedetector.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/opencv.hpp"

class Finder : public QObject
{
    Q_OBJECT
public:
    explicit Finder(Settings* settings, QObject *parent = nullptr);

signals:
    void findImage(FigureType type, QRect rect);

public slots:
    void detect(QPixmap pixmap);

private:
    const int COLORTOLERANCE = 100;
    const int MAXSQUARE = 1000000;
    const int MINSQUARE = 10000;
    const QString names[3] = {"RED", "YELLOW", "BLUE"};

    struct Color{
        int hMax, hMin, sMax, sMin, vMax, vMin;
    };

    bool _detectFigure(QPixmap pixmap);
    bool _detectText(QPixmap pixmap);
    void _loadDescriptors();
    void _loadSettings();
    void _initConnections();

    template<class T>
    float _getSquare(std::vector<T> poitns){
        float result = 0;
        for (size_t i = 0; i < poitns.size(); i++){
            result += poitns[i].x * poitns[(i + 1) % poitns.size()].y;
            result -= poitns[i].y * poitns[(i + 1) % poitns.size()].x;
        }
        return qAbs(result);
    }

    FigureColor _getFigureColor(QColor color);
    QRect _getRect(std::vector<cv::Point2f> poitns);
    cv::Mat _getMask(QPixmap pixmap, int index);
    cv::Mat _getGrauScaleMat(QImage image);

    FigureType _type;
    QRect _rect;
    Settings* _settings;
    Color _colors[3];
    cv::Mat _descriptors[6];
    std::vector<cv::KeyPoint> _keypoints[6];
    cv::Mat _img_objects[6];
};

#endif // FINDER_HPP
