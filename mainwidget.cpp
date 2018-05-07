#include "mainwidget.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QThread>
#include <QDoubleValidator>
#include <QDebug>

#include <video/videostreamparser.hpp>

MainWidget::MainWidget(QWidget *parent) :
    QTabWidget(parent),
    _settings(new QSettings(".ini", QSettings::IniFormat)),
    _cameraWidget(new CameraWidget(_settings, this)),
    _calculationWidget(new CalculationWidget(this)),
    _obsWidget(new ObsWidget(this)),
    _configWidget(new ConfigWidget(_settings, this))
{
    _createLayout();
    _initConnetions();
    _initVideoParser();
}

bool MainWidget::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::MouseButtonPress)
        qDebug() << obj->staticQtMetaObject.className();
}

void MainWidget::_createLayout()
{
//    addTab(new CalculationWidget(this), "Залепа");
    addTab(_cameraWidget, "Камера");
    addTab(_calculationWidget, "Вычисления");
    addTab(_obsWidget, "Сейсмометр");
    addTab(_configWidget, "Конфиг");
//    setTabEnabled(0, false);
}

void MainWidget::_initConnetions()
{

}

void MainWidget::_initVideoParser()
{
    QThread* thread = new QThread;
    VideoStreamParser* parser = new VideoStreamParser(URL);

    parser->moveToThread(thread);

    connect(thread, &QThread::started, parser, &VideoStreamParser::process);
    connect(parser, &VideoStreamParser::finished, thread, &QThread::quit);
    connect(parser, &VideoStreamParser::repaint, _cameraWidget, &CameraWidget::UpdateCamera);
    connect(parser, &VideoStreamParser::repaint, _configWidget, &ConfigWidget::UpdateCamera);
    connect(parser, &VideoStreamParser::finished, parser, &VideoStreamParser::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();
}
