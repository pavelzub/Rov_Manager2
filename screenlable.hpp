#ifndef SCREENLABLE_HPP
#define SCREENLABLE_HPP

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPixmap>

class ScreenLabel : public QLabel
{
public:
    ScreenLabel(QPixmap pixmap, QWidget *parent = nullptr);
    void clearImage();
    void setLineLength(QString value);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    const int WIDTH = 16;
    const int HEIGHT = 9;
    const int LABELWIDTH = 700;

    QPixmap _pixmap;
    QLineF _mainLine = {0, 0, 0, 0};
    QLineF _subLine = {0, 0, 0, 0};
    bool _isDrowMain = false;
    bool _isDrowSub = false;
    double _mainLength = 1;
};

#endif // SCREENLABLE_HPP
