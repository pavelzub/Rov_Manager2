#include "finder.hpp"
#include <QRgb>
#include <algorithm>
#include <iostream>
#include <QDebug>
#define sqr(x) ((x)*(x))

Finder::Finder(QSettings *settings, Type *type, QRect *rect, QObject *parent) : QObject(parent)
{
    _settings = settings;
    _type = type;
    _rect = rect;
    _loadSettings();
}

void Finder::detect(QPixmap pixmap)
{
    *_type = NONE;
    _detectText(pixmap) || _detectFigure(pixmap);

    emit findImage();
}

bool Finder::_detectFigure(QPixmap pixmap)
{
    std::vector<std::vector<cv::Point> > contours;
    for (int i = 0; i < 1; i++){
        cv::Mat mask = _getMask(pixmap, i);
//        double cannyParams = cv::threshold(mask, mask, 0, 10, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);
//        cv::Canny(mask, mask, cannyParams, cannyParams / 2.0);
        cv::findContours(mask, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
        cv::Mat black = cv::Mat::zeros(mask.rows, mask.cols, mask.type());
        cv::drawContours(black, contours, -1, cv::Scalar(100,200,255));
        cv::imshow("3", black);
        cv::waitKey(1);

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
            qDebug() << "1";
//            cv::drawContours(mask, contours.at(i), 0, cv::Scalar(100,200,255));

            QPoint min = {INT_MAX, INT_MAX};
            QPoint max = {0, 0};
            for (auto i : hull){
                min.setX(qMin(min.x(), i.x));
                min.setY(qMin(min.y(), i.y));
                max.setX(qMax(max.x(), i.x));
                max.setY(qMax(max.y(), i.y));
            }
            *_rect = QRect(min, max);
            *_type = static_cast<Type>(1);
            return true;
        }
//        cv::imshow("2", mask);
//        cv::waitKey(1);
    }

    return false;
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
            cv::Mat img_object = cv::imread("templates//" + TEMPLATESPATH[i].toStdString(), CV_LOAD_IMAGE_GRAYSCALE);

            std::vector<cv::KeyPoint> keypoints_object;
            detector.detect(img_object, keypoints_object);

            cv::Mat descriptors_object;
            extractor.compute(img_object, keypoints_object, descriptors_object);

            cv::FlannBasedMatcher matcher;
            cv::vector< cv::DMatch > matches;
            matcher.match( descriptors_object, descriptors_scene, matches );

            float max_dist = 0; float min_dist = 100;

            for(size_t i = 0; i < static_cast<size_t>(descriptors_object.rows); i++ )
            {
                float dist = matches[i].distance;
                if( dist < min_dist ) min_dist = dist;
                if( dist > max_dist ) max_dist = dist;
            }

            std::vector< cv::DMatch > good_matches;

            for(size_t i = 0; i < static_cast<size_t>(descriptors_object.rows); i++ )
            {
                if( matches[i].distance < 3 * min_dist )
                {
                    good_matches.push_back( matches[i]);
                }
            }

            std::vector<cv::Point2f> obj;
            std::vector<cv::Point2f> scene;

            for(size_t i = 0; i < good_matches.size(); i++ )
            {
                obj.push_back( keypoints_object[static_cast<size_t>(good_matches[i].queryIdx)].pt );
                scene.push_back( keypoints_scene[static_cast<size_t>(good_matches[i].trainIdx)].pt );
            }

            if (obj.size() * scene.size() <= 16) return false;
            cv::Mat H = cv::findHomography(obj, scene, CV_RANSAC);
            std::vector<cv::Point2f> obj_corners(4);
            obj_corners[0] = cvPoint(0,0);
            obj_corners[1] = cvPoint( img_object.cols, 0 );
            obj_corners[2] = cvPoint( img_object.cols, img_object.rows );
            obj_corners[3] = cvPoint( 0, img_object.rows );

            std::vector<cv::Point2f> scene_corners(4);
            cv::perspectiveTransform(obj_corners, scene_corners, H);

            float square = _getSquare(scene_corners);
            if (cv::isContourConvex(scene_corners) && square > MINSQUARE && square < MAXSQUARE){
                *_rect = _getRect(scene_corners);
                *_type = static_cast<Type>(i + 1);
                return true;
            }
        }
    } catch (...) {
    }
    return false;
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

    image = image + image1;
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

float Finder::_getSquare(std::vector<cv::Point2f> poitns)
{
    float result = 0;
    for (size_t i = 0; i < poitns.size(); i++){
        result += poitns[i].x * poitns[(i + 1) % poitns.size()].y;
        result -= poitns[i].y * poitns[(i + 1) % poitns.size()].x;
    }
    return qAbs(result);
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
