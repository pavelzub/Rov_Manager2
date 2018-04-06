#ifndef SCREENSHOTWINDOW_HPP
#define SCREENSHOTWINDOW_HPP

#include <QLineEdit>
#include <QMainWindow>
#include <QObject>
#include <QPixmap>
#include <QPushButton>

#include "screenlable.hpp"

class ScreenshotWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ScreenshotWindow(QPixmap pixmap, QWidget *parent = nullptr);

private:
    void _createLayouts();
    void _initConnections();

    ScreenLabel* _screenShot;
    QLineEdit* _textEdit;
    QPushButton* _clearBtn;
};

#endif // SCREENSHOTWINDOW_HPP
