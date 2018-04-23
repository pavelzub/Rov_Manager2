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
    void buildPlot();

private:
    void _createLayout();

    DataStore* _dataStore;
    QCustomPlot* _plot;
};

#endif // OBSWIDGET_HPP
