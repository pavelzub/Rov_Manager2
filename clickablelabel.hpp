#ifndef CLICKABLELABEL_HPP
#define CLICKABLELABEL_HPP

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPixmap>

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    ClickableLabel(QPixmap* pixmap, QWidget *parent = nullptr);

signals:
    void clicked(int h, int s, int v);

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    QPixmap* _pixmap;
};

#endif // CLICKABLELABEL_HPP
