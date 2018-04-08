#include "mainwindow.hpp"
#include <iostream>
#include <QAction>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _mainwidget(new MainWidget(this)),
      _calculationWindow(new CalculationWindow(this))
{
    this->setCentralWidget(_mainwidget);
    _createMenu();
    _initConnections();
}

MainWindow::~MainWindow()
{
}

void MainWindow::_createMenu()
{
    QAction* crashAct = new QAction("Расчеты падения", this);
    auto fileMenu = menuBar()->addMenu("Расчеты");
    fileMenu->addAction(crashAct);

    connect(crashAct, &QAction::triggered, [this](){
        _calculationWindow->show();
    });
}

void MainWindow::_initConnections()
{

}
