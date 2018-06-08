#ifndef CAMERAWIDGET_HPP
#define CAMERAWIDGET_HPP

#include <QWidget>
#include <QPushButton>

#include "video/videowidget.hpp"
#include "settings.hpp"

class CameraWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CameraWidget(Settings* settings, QWidget *parent = nullptr);
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
