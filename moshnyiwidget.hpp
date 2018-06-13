#ifndef MOSHNYIWIDGET_HPP
#define MOSHNYIWIDGET_HPP

#include <QObject>
#include <QWidget>
#include <QLabel>

class MoshnyiWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MoshnyiWidget(QWidget *parent = nullptr);
    void setDepth(float yaw, float pitch, float roll, float depth);

private:
    void _createLayout();
    QLayout* _createTurboLayout();
    QLayout* _createDepthLayout();
    QLabel* _depth;
};

#endif // MOSHNYIWIDGET_HPP
