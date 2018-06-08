#ifndef MAP_HPP
#define MAP_HPP

#include <QObject>
#include <QWidget>
#include <QLabel>

class Map : public QLabel
{
public:
    Map(QWidget *parent);
    void setLine(QLineF line);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

protected:
    QLineF _line;
    QPoint _point;
};

#endif // MAP_HPP
