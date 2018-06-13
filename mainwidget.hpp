#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QWidget>
#include <QTabWidget>

#include "camerawidget.hpp"
#include "calculationwidget.hpp"
#include "obswidget.hpp"
#include "configwidget.hpp"
#include "settings.hpp"
#include "obsconfigwidget.hpp"
#include "moshnyiwidget.hpp"
#include "ethernet/datastore.hpp"

class MainWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);

private:
    void _createLayout();
    void _initConnetions();
    void _initVideoParser();

    const QString URL = "192.168.88.228:3090";
//    const QString URL = "udp://192.168.1.1:1234";
    Settings* _settings;
    CameraWidget* _cameraWidget;
    CalculationWidget* _calculationWidget;
    ObsWidget* _obsWidget;
    ConfigWidget* _configWidget;
    ObsConfigWidget* _obsConfigWidget;
    MoshnyiWidget* _moshnyiWidget;
    DataStore* _dataStore;
};

#endif // MAINWIDGET_HPP
