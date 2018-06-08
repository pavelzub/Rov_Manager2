#ifndef MOSHNYIWIDGET_HPP
#define MOSHNYIWIDGET_HPP

#include <QObject>
#include <QWidget>

class MoshnyiWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MoshnyiWidget(QWidget *parent = nullptr);

private:
    void _createLayout();
};

#endif // MOSHNYIWIDGET_HPP
