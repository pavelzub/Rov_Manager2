#include "obswidget.hpp"
#include "QVBoxLayout"
#include <QVector>
#include <vector>

ObsWidget::ObsWidget(QWidget *parent) :
    QWidget(parent),
    _plot(new QCustomPlot)
{
    _createLayout();
}

void ObsWidget::buildPlot(QVector<double> data)
{
    QVector<double> x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    _plot->graph(0)->setData(x, data);

    _plot->xAxis->setLabel("x");
    _plot->yAxis->setLabel("y");
    _plot->xAxis->setRange(0, 17);
    _plot->yAxis->setRange(-8, 8);
    _plot->replot();
}

void ObsWidget::_createLayout()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_plot);
    this->setLayout(layout);
    _plot->addGraph();
}
