#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include "mainwidget.hpp"
#include "calculationwindow.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void _createMenu();
    void _initConnections();

    MainWidget* _mainwidget;
    CalculationWindow* _calculationWindow;
};

#endif // MAINWINDOW_HPP
