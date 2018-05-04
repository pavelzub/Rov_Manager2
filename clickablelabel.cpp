#include "clickablelabel.hpp"
#include <QMouseEvent>
#include <QDebug>

ClickableLabel::ClickableLabel(QPixmap *pixmap, QWidget *parent):
    QLabel (parent)
{
    _pixmap  = pixmap;
}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    int h, s, v;
    QColor c = _pixmap->toImage().pixel(event->x(), event->y());
    c.getHsv(&h, &s, &v);
    qDebug() << h << " " << s << " " << v;
    emit clicked(h / 2, s, v);
}
