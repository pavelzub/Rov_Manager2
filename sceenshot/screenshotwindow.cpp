#include "screenshotwindow.hpp"
#include <QDoubleValidator>
#include <QVBoxLayout>
#include <QHBoxLayout>

ScreenshotWindow::ScreenshotWindow(QPixmap pixmap, QWidget *parent) :
    QMainWindow(parent),
    _screenShot(new ScreenLabel(pixmap, this)),
    _textEdit(new QLineEdit(this)),
    _clearBtn(new QPushButton(this))
{
    move(0, 0);
    setFixedSize(_screenShot->width(), _screenShot->height() + 20);
    setAttribute(Qt::WA_DeleteOnClose, true);
    _createLayouts();
    _initConnections();
}

void ScreenshotWindow::_createLayouts()
{
    QWidget* w = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* textLayout = new QHBoxLayout;
    QLabel* lable(new QLabel(this));

    lable->setText("Длинна основного отрезка: ");
    lable->setAlignment(Qt::AlignHCenter);
    lable->setStyleSheet("font: 12pt;");
    _textEdit->setValidator(new QDoubleValidator(0, 100, 4, this));
    _clearBtn->setText("Очистить");

    textLayout->addWidget(lable);
    textLayout->addWidget(_textEdit);
    textLayout->addSpacing(1);
    textLayout->addWidget(_clearBtn);
    mainLayout->addLayout(textLayout);
    mainLayout->setMargin(0);
    mainLayout->addWidget(_screenShot);
    w->setLayout(mainLayout);
    setCentralWidget(w);
}

void ScreenshotWindow::_initConnections()
{
    connect(_clearBtn, &QPushButton::pressed, _screenShot, &ScreenLabel::clearImage);
    connect(_textEdit, &QLineEdit::textChanged, _screenShot, &ScreenLabel::setLineLength);
}
