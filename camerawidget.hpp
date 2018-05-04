#ifndef CAMERAWIDGET_HPP
#define CAMERAWIDGET_HPP

#include <QWidget>
#include <QPushButton>
#include <QSettings>

#include "video/videowidget.hpp"

class CameraWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CameraWidget(QSettings* settings, QWidget *parent = nullptr);
    void UpdateCamera(QPixmap pixmap);

private:
    void _createLayout();
    void _initConnections();
    void _buttonScreenPress();
    void _buttonSearchPress();

    QPushButton* _screenBtn;
    QPushButton* _searchBtn;
    VideoWidget* _videoWidget;
};

#endif // CAMERAWIFGET_HPP
