#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QWidget>
#include <QTabWidget>
#include "camerawidget.hpp"
#include "calculationwidget.hpp"
#include "obswidget.hpp"

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
    CameraWidget* _cameraWidget;
    CalculationWidget* _calculationWidget;
    ObsWidget* _obsWidget;
};

#endif // MAINWIDGET_HPP
