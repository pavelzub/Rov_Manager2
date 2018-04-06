#include "mainwindow.hpp"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _mainwidget(new MainWidget(this))
{
    this->setCentralWidget(_mainwidget);
    initializeConnections();

//  //////  this->setMinimumSize(QSize(800, 500));
   // this->setMaximumSize(QSize(800, 500));
}

MainWindow::~MainWindow()
{
}

void MainWindow::initializeConnections()
{

}
