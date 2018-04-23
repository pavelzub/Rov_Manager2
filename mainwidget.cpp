#include "mainwidget.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QThread>
#include <QDoubleValidator>

#include <video/videostreamparser.hpp>

MainWidget::MainWidget(QWidget *parent) :
    QTabWidget(parent),
    _cameraWidget(new CameraWidget(this)),
    _calculationWidget(new CalculationWidget(this)),
    _obsWidget(new ObsWidget(this))
{
    _createLayout();
    _initConnetions();
    _initVideoParser();
    _initVideoParser();
}

void MainWidget::_createLayout()
{
    addTab(new CameraWidget(this), "Залепа");
    addTab(_cameraWidget, "Камера");
    addTab(_calculationWidget, "Вычисления");
    addTab(_obsWidget, "Сейсмометр");
    setTabEnabled(0, false);
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
    connect(parser, &VideoStreamParser::finished, parser, &VideoStreamParser::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();
}
