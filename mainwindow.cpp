#include "mainwindow.hpp"
#include <iostream>
#include <QAction>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _mainwidget(new MainWidget(this))
{
    this->setCentralWidget(_mainwidget);
}

MainWindow::~MainWindow()
{
}
