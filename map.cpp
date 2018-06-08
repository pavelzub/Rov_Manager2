#include "map.hpp"

#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

Map::Map(QWidget *parent):
    QLabel (parent)
{
    setFixedSize(1072.0, 567.0);
    setScaledContents(true);
    setPixmap(QPixmap("map.jpg"));
}

void Map::setLine(QLineF line)
{
    double k = (569.0 / 190.0) * 5;
    double h = (height()  / k) / 1000;
    _line = QLine({0, 0}, {static_cast<int>(line.x2() * h), static_cast<int>(-line.y2() * h)});
}

void Map::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 5));
    auto line = _line;
    line.setLength(line.length() - 7);
    painter.drawLine(_point, {static_cast<int>(_point.x() + line.x2()),static_cast<int>(_point.y() + line.y2())});
    painter.setPen(QPen(Qt::red, 4));
    painter.drawEllipse(_point + _line.p2(), 7, 7);

    painter.end();
//    qDebug() << _point.x() << " " << _point.y() << " " << _point.x() + _line.x2() << " " << _point.y() + _line.y2();
}

void Map::mousePressEvent(QMouseEvent *event)
{
    _point = event->pos();
    repaint();
}
