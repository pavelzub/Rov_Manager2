#include "obswidget.hpp"
#include "QVBoxLayout"
#include <QVector>
#include <vector>

ObsWidget::ObsWidget(QWidget *parent) :
    QWidget(parent),
    _dataStore(new DataStore(this)),
    _plot(new QCustomPlot)
{
    _createLayout();
    buildPlot();
}

void ObsWidget::buildPlot()
{
    std::vector<double> y = { 0, 4.4, -4.3, 0, 0, 6.8, -7, 0, 0, 1.6, -1.6, 0, 0, 6.3, -6.4, 0};
    QVector<double> x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    _plot->graph(0)->setData(x, QVector<double>::fromStdVector(y));

    _plot->xAxis->setLabel("x");
    _plot->yAxis->setLabel("y");
    _plot->xAxis->setRange(0, 17);
    _plot->yAxis->setRange(-8, 8);
}

void ObsWidget::_createLayout()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_plot);
    this->setLayout(layout);
    _plot->addGraph();
}
