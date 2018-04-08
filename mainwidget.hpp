#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include "video/videowidget.hpp"
#include "sceenshot/screenshotwindow.hpp"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);

private:
    void createLayout();
    void initializeConnetions();
    void buttonScreenPress();
    void buttonSearchPress();
    QPushButton* _screenBtn;
    QPushButton* _searchBtn;
    VideoWidget* _videoWidget;

};

#endif // MAINWIDGET_HPP
