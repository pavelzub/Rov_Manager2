#ifndef OBSWIDGET_HPP
#define OBSWIDGET_HPP

#include <QWidget>
#include "qcustomplot/qcustomplot.hpp"
#include "ethernet/datastore.hpp"

class ObsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ObsWidget(QWidget *parent = nullptr);
    void buildPlot(QVector<double> data);

private:
    void _createLayout();

    QCustomPlot* _plot;
};

#endif // OBSWIDGET_HPP
