#include "finder.hpp"
#include <QRgb>
#include <algorithm>
#include <iostream>
#include <QDebug>
#define sqr(x) ((x)*(x))

Finder::Finder(Settings *settings, QObject *parent) : QObject(parent)
{
    _settings = settings;
    _loadSettings();
    _loadDescriptors();
    _initConnections();
}

void Finder::detect(QPixmap pixmap)
{
    _type = NONE;
    _detectText(pixmap) || _detectFigure(pixmap);

    emit findImage(_type, _rect);
}

bool Finder::_detectFigure(QPixmap pixmap)
{
    std::vector<std::vector<cv::Point> > contours;
    float squere = 0;
    QRect r;
    FigureType t = NONE;
    for (int j = 0; j < 3; j++){
        cv::Mat mask = _getMask(pixmap, j);
        cv::findContours(mask, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
        for (std::size_t i = 0; i < contours.size(); i++)
        {
            if (contours.at(i).size() < 5) continue;
            if (std::fabs(cv::contourArea(contours.at(i))) < 1200.0) continue;
            static std::vector<cv::Point> hull;
            cv::convexHull(contours.at(i), hull, true);
            cv::approxPolyDP(hull, hull, 15, true);
            if (!cv::isContourConvex(hull)) continue;
            cv::RotatedRect bEllipse = cv::fitEllipse(contours.at(i));
            if (hull.size() > 4) continue;

            float tmp = cv::contourArea(hull);
            if (tmp < squere) break;

            squere = tmp;
            QPoint min = {INT_MAX, INT_MAX};
            QPoint max = {0, 0};
            for (auto i : hull){
                min.setX(qMin(min.x(), static_cast<int>(i.x)));
                min.setY(qMin(min.y(), static_cast<int>(i.y)));
                max.setX(qMax(max.x(), static_cast<int>(i.x)));
                max.setY(qMax(max.y(), static_cast<int>(i.y)));
            }
            r = QRect(min, max);
            t = static_cast<FigureType>(1 + (hull.size() - 3) * 3 + j);
        }
    }

    _rect = r;
    _type = t;
    return _type == NONE;
}

bool Finder::_detectText(QPixmap pixmap)
{
    try {
        cv::Mat img_scene = _getGrauScaleMat(pixmap.toImage());
        std::vector<cv::KeyPoint> keypoints_scene;

        int minHessian = 400;
        cv::SurfFeatureDetector detector(minHessian);
        detector.detect(img_scene, keypoints_scene);

        cv::SurfDescriptorExtractor extractor;
        cv::Mat descriptors_scene;
        extractor.compute(img_scene, keypoints_scene, descriptors_scene);

        for (int i = 0; i < 6; i++){
            cv::FlannBasedMatcher matcher;
            cv::vector< cv::DMatch > matches;
            matcher.match( _descriptors[i], descriptors_scene, matches );

            float max_dist = 0; float min_dist = 100;

            for(size_t i = 0; i < static_cast<size_t>(_descriptors[i].rows); i++ )
            {
                float dist = matches[i].distance;
                if( dist < min_dist ) min_dist = dist;
                if( dist > max_dist ) max_dist = dist;
            }

            std::vector< cv::DMatch > good_matches;

            for(size_t i = 0; i < static_cast<size_t>(_descriptors[i].rows); i++ )
            {
                if( matches[i].distance < 3 * min_dist )
                {
                    good_matches.push_back( matches[i]);
                }
            }

            std::vector<cv::Point2f> obj;
            std::vector<cv::Point2f> scene;

            for(size_t j = 0; j < good_matches.size(); j++ )
            {
                obj.push_back( _keypoints[i][static_cast<size_t>(good_matches[j].queryIdx)].pt );
                scene.push_back( keypoints_scene[static_cast<size_t>(good_matches[j].trainIdx)].pt );
            }

            if (obj.size() * scene.size() <= 16) return false;
            cv::Mat H = cv::findHomography(obj, scene, CV_RANSAC);
            std::vector<cv::Point2f> obj_corners(4);
            obj_corners[0] = cvPoint(0,0);
            obj_corners[1] = cvPoint( _img_objects[i].cols, 0 );
            obj_corners[2] = cvPoint( _img_objects[i].cols, _img_objects[i].rows );
            obj_corners[3] = cvPoint( 0, _img_objects[i].rows );

            std::vector<cv::Point2f> scene_corners(4);
            cv::perspectiveTransform(obj_corners, scene_corners, H);

            float square = _getSquare(scene_corners);
            if (cv::isContourConvex(scene_corners) && square > MINSQUARE && square < MAXSQUARE){
                _rect = _getRect(scene_corners);
                _type = static_cast<FigureType>(i + 1);
                return true;
            }
        }
    } catch (...) {
    }
    return false;
}

void Finder::_loadDescriptors()
{
    cv::SurfFeatureDetector detector(400);
    cv::SurfDescriptorExtractor extractor;

    for (int i = 0; i < 6; i++){
        _img_objects[i] = cv::imread("templates//" + TEMPLATESPATH[i].toStdString(), CV_LOAD_IMAGE_GRAYSCALE);
        qDebug() << "1";
        detector.detect(_img_objects[i], _keypoints[i]);
        extractor.compute(_img_objects[i], _keypoints[i], _descriptors[i]);
    }
}

cv::Mat Finder::_getMask(QPixmap pixmap, int index)
{
    QImage imag = pixmap.copy(pixmap.rect()).toImage();
    cv::Mat black = cv::Mat::zeros(imag.height(),imag.width(),CV_8UC4);
    cv::Mat img(imag.height(),imag.width(),CV_8UC4,
                static_cast<void*>(const_cast<uchar *>(imag.constBits())),
                static_cast<size_t>(imag.bytesPerLine()));

    int hMin = _colors[index].hMin;
    int hMax = _colors[index].hMax;
    int sMin = _colors[index].sMin;
    int sMax = _colors[index].sMax;
    int vMin = _colors[index].vMin;
    int vMax = _colors[index].vMax;

    if (hMin > 180){
        hMin -= 180;
        hMax -= 180;
    }
    cv::Mat image = img.clone();
    cv::cvtColor(image, image, CV_BGR2HSV);
    cv::Scalar upper(MIN(hMax, 180), sMax, vMax);
    cv::Scalar lower(hMin, sMin, vMin);
    cv::inRange(image, lower, upper, image);

    cv::Mat image1 = img.clone();
    cv::cvtColor(image1, image1, CV_BGR2HSV);
    cv::Scalar upper1(MAX(hMax - 180, 0), sMax, vMax);
    cv::Scalar lower1(0, sMin, vMin);
    cv::inRange(image1, lower1, upper1, image1);

    image = image | image1;
    cv::imshow(QString::number(index).toStdString(), image);
    cv::waitKey(1);

    return image;
}

void Finder::_loadSettings()
{
    for (int i = 0; i < 3; i++){
        _settings->beginGroup(names[i]);
        _colors[i].hMax = _settings->value("h_high", 360).toInt();
        _colors[i].hMin = _settings->value("h_low", 0).toInt();
        _colors[i].sMax = _settings->value("s_high", 255).toInt();
        _colors[i].sMin = _settings->value("s_low", 0).toInt();
        _colors[i].vMax = _settings->value("v_high", 255).toInt();
        _colors[i].vMin = _settings->value("v_low", 0).toInt();
        _settings->endGroup();
    }
}

void Finder::_initConnections()
{
    connect(_settings, &Settings::settingsUpdate, this, &Finder::_loadSettings);
}

QRect Finder::_getRect(std::vector<cv::Point2f> poitns)
{
    QPoint min = {INT_MAX, INT_MAX};
    QPoint max = {0, 0};
    for (auto i : poitns){
        min.setX(qMin(min.x(), static_cast<int>(i.x)));
        min.setY(qMin(min.y(), static_cast<int>(i.y)));
        max.setX(qMax(max.x(), static_cast<int>(i.x)));
        max.setY(qMax(max.y(), static_cast<int>(i.y)));
    }
    return QRect(min, max);
}

cv::Mat Finder::_getGrauScaleMat(QImage image)
{
    cv::Mat mat(image.height(),image.width(),CV_8UC4,
                static_cast<void*>(const_cast<uchar *>(image.constBits())),
                static_cast<size_t>(image.bytesPerLine()));
    cv::cvtColor(mat, mat, CV_BGR2GRAY);
    cv::GaussianBlur(mat, mat, cv::Size(5, 5), cv::BORDER_REFLECT);
    return mat;
}
