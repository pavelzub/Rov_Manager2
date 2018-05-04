#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QWidget>
#include <QTabWidget>
#include <QSettings>

#include "camerawidget.hpp"
#include "calculationwidget.hpp"
#include "obswidget.hpp"
#include "configwidget.hpp"

class MainWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);

private:
    void _createLayout();
    void _initConnetions();
    void _initVideoParser();

    const QString URL = "udp://192.168.1.255:1234";
    QSettings* _settings;
    CameraWidget* _cameraWidget;
    CalculationWidget* _calculationWidget;
    ObsWidget* _obsWidget;
    ConfigWidget* _configWidget;
};

#endif // MAINWIDGET_HPP
