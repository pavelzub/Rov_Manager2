#ifndef CONFIGWIDGET_HPP
#define CONFIGWIDGET_HPP

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QComboBox>

#include "settings.hpp"
#include "ctkrangeslider.hpp"
#include "clickablelabel.hpp"

class ConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigWidget(Settings* setting, QWidget *parent = nullptr);
    void UpdateCamera(QPixmap pixmap);

private:
    void _createLayout();
    void _initConnections();
    void _loadSettings();
    void _saveConfig();
    void _setIntervals(int h, int s, int v);

    QPixmap* _pixmap;
    ClickableLabel* _image;
    QPushButton* _pipetBtn;
    QPushButton* _saveBtn;
    ctkRangeSlider* _sliders[3];
    Settings* _settings;
    QComboBox* _comboBox;
};

#endif // CONFIGWIDGET_HPP
