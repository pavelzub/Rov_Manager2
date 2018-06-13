#include "mainwidget.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QThread>
#include <QDoubleValidator>
#include <QDebug>
#include "video/websocket.hpp"
#include "video/videostreamparser.hpp"

MainWidget::MainWidget(QWidget *parent) :
    QTabWidget(parent),
    _settings(new Settings(".ini", QSettings::IniFormat)),
    _cameraWidget(new CameraWidget(_settings, this)),
    _calculationWidget(new CalculationWidget(this)),
    _obsWidget(new ObsWidget(this)),
    _configWidget(new ConfigWidget(_settings, this)),
    _obsConfigWidget(new ObsConfigWidget(this)),
    _moshnyiWidget(new MoshnyiWidget(this)),
    _dataStore(new DataStore(this))
{
    _createLayout();
    _initConnetions();
    _initVideoParser();
}

void MainWidget::_createLayout()
{
    addTab(_cameraWidget, "Камера");
    addTab(_calculationWidget, "Точка падения");
    addTab(_moshnyiWidget, "Расчет мощности");
    addTab(_obsWidget, "Сейсмометр");
    addTab(_obsConfigWidget, "Настройка");
    addTab(_configWidget, "Конфиг");
}

void MainWidget::_initConnetions()
{
    connect(_obsConfigWidget, &ObsConfigWidget::newObsData, _obsWidget, &ObsWidget::buildPlot);
    connect(_dataStore, &DataStore::telimetryUpdate, _moshnyiWidget, &MoshnyiWidget::setDepth);
}

void MainWidget::_initVideoParser()
{
    QThread* thread = new QThread;

    if (URL[0] == "u"){
        av_register_all();
        avformat_network_init();
        avcodec_register_all();

        VideoStreamParser* parser = new VideoStreamParser(URL);
        parser->moveToThread(thread);
        connect(thread, &QThread::started, parser, &VideoStreamParser::process);
        connect(parser, &VideoStreamParser::finished, thread, &QThread::quit);
        connect(parser, &VideoStreamParser::repaint, _cameraWidget, &CameraWidget::UpdateCamera);
        connect(parser, &VideoStreamParser::repaint, _configWidget, &ConfigWidget::UpdateCamera);
        connect(parser, &VideoStreamParser::finished, parser, &VideoStreamParser::deleteLater);
    }
    else
    {
        WebSocket* socket = new WebSocket(URL);

        socket->moveToThread(thread);

        connect(thread, &QThread::started, socket, &WebSocket::process);
        connect(socket, &WebSocket::newFrame, _cameraWidget, &CameraWidget::UpdateCamera);
        connect(socket, &WebSocket::newFrame, _configWidget, &ConfigWidget::UpdateCamera);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    }
    thread->start();
}
