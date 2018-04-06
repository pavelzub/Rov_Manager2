#include "screenlable.hpp"

#include <QMouseEvent>
#include <QPainter>

ScreenLabel::ScreenLabel(QPixmap pixmap, QWidget *parent):
    QLabel(parent)
{
    _pixmap = pixmap;
    setFixedSize(LABELWIDTH, LABELWIDTH * HEIGHT / WIDTH);
    setMouseTracking(true);
}

void ScreenLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    QPainter painter(this);
    if (_pixmap.size() != QSize(0, 0)) painter.drawPixmap(rect(), _pixmap);

    QFont font = painter.font();
    font.setPointSize (10);
    painter.setFont(font);

    QPen mainPen(Qt::blue);
    mainPen.setWidth(2);
    painter.setPen(mainPen);
    painter.drawLine(_mainLine);
    mainPen = QPen(Qt::red);
    painter.setPen(mainPen);
    QPointF center = _mainLine.center();
    QPointF offset = QPointF((_mainLine.p2().y() - _mainLine.p1().y()),
                             -(_mainLine.p2().x() - _mainLine.p1().x()));
    double k = sqrt(offset.x() * offset.x() + offset.y() * offset.y());
    offset = {offset.x() / k * 10.0, offset.y() / k *10.0};
    center += offset;
    painter.drawText(center,  QString::number(_mainLength));

    QPen subPen(Qt::black);
    subPen.setWidth(2);
    painter.setPen(subPen);
    painter.drawLine(_subLine);
    subPen = QPen(Qt::red);
    painter.setPen(subPen);
    center = _subLine.center();
    offset = QPointF((_subLine.p2().y() - _subLine.p1().y()),
                             -(_subLine.p2().x() - _subLine.p1().x()));
    k = sqrt(offset.x() * offset.x() + offset.y() * offset.y());
    offset = {offset.x() / k * 10.0, offset.y() / k *10.0};
    center += offset;
    painter.drawText(center, QString::number(_subLine.length() / _mainLine.length() * _mainLength));

    painter.end();

    return;
}

void ScreenLabel::clearImage()
{
    _mainLine = {0, 0, 0, 0};
    _subLine = {0, 0, 0, 0};
    repaint();
}

void ScreenLabel::setLineLength(QString value)
{
    _mainLength = value.replace(",", ".").toDouble();
    repaint();
}

void ScreenLabel::mouseMoveEvent(QMouseEvent *event)
{
    QPointF point = event->localPos();
    if (_isDrowMain)
        _mainLine.setP2(point);
    else if (_isDrowSub){
        _subLine.setP2(point);
    }

    repaint();
    return;
}

void ScreenLabel::mousePressEvent(QMouseEvent *event)
{
    QPointF point = event->localPos();
    if (event->button() == Qt::MouseButton::RightButton && !_isDrowMain && !_isDrowSub){
        _mainLine.setP1(point);
        _mainLine.setP2(point);
        _isDrowMain = true;
    }
    else if (_isDrowMain || _isDrowSub){
            _isDrowMain = _isDrowSub = false;
    }
    else{
        _subLine.setP1(point);
        _subLine.setP2(point);
        _isDrowSub = true;
    }
}
