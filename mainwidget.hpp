#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include "videowidget.hpp"
#include "screenshotwindow.hpp"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);

private:
    void createLayout();
    void initializeConnetions();
    void buttonScreenPress();
    QPushButton* _screenBtn;
    VideoWidget* _videoWidget;

};

#endif // MAINWIDGET_HPP
