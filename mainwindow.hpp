#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include "mainwidget.hpp"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    MainWidget* _mainwidget;
    void initializeConnections();
};

#endif // MAINWINDOW_HPP
