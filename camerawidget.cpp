#include "camerawidget.hpp"
#include <sceenshot/screenshotwindow.hpp>

#include <QVBoxLayout>

CameraWidget::CameraWidget(Settings *settings, QWidget *parent) :
    QWidget(parent),
    _screenBtn(new QPushButton(this)),
    _searchBtn(new QPushButton(this)),
    _videoWidget(new VideoWidget(settings, this))
{
    _createLayout();
    _initConnections();
}

void CameraWidget::UpdateCamera(QPixmap pixmap)
{
    _videoWidget->Update(pixmap);
}

void CameraWidget::_createLayout()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    QHBoxLayout* camLayout = new QHBoxLayout;
    QVBoxLayout* menuLayout = new QVBoxLayout;

    camLayout->addWidget(_videoWidget);
    _screenBtn->setText("Скриншот");
    _searchBtn->setText("Начать распознавание");

    menuLayout->addWidget(_screenBtn);
    menuLayout->addWidget(_searchBtn);
    mainLayout->addLayout(camLayout);
    mainLayout->addLayout(menuLayout);
    this->setLayout(mainLayout);
}

void CameraWidget::_initConnections()
{
    connect(_screenBtn, &QPushButton::pressed, this, &CameraWidget::_buttonScreenPress);
    connect(_searchBtn, &QPushButton::pressed, this, &CameraWidget::_buttonSearchPress);
}

void CameraWidget::_buttonScreenPress()
{
    ScreenshotWindow* w(new ScreenshotWindow(_videoWidget->GetPixmap(), this));
    w->show();
}

void CameraWidget::_buttonSearchPress()
{
    if (_searchBtn->text() == "Начать распознавание"){
        _searchBtn->setText("Остановить распознавание");
        _videoWidget->StartStopRecognition(true);
    }
    else {
        _searchBtn->setText("Начать распознавание");
        _videoWidget->StartStopRecognition(false);
    }
}
