#include "configwidget.hpp"
#include <QHBoxLayout>
#include "opencv2/core/core.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/opencv.hpp"
#include <QDebug>
#include <QPainter>

ConfigWidget::ConfigWidget(QSettings *setting, QWidget *parent) :
    QWidget(parent),
    _pixmap(new QPixmap),
    _image(new ClickableLabel(_pixmap, this)),
    _pipetBtn(new QPushButton(this)),
    _saveBtn(new QPushButton(this)),
    _comboBox(new QComboBox(this))
{
    _settings = setting;
    _createLayout();
    _initConnections();
    _loadSettings();
}

void ConfigWidget::_createLayout()
{
    QHBoxLayout* layout = new  QHBoxLayout(this);
    QVBoxLayout* menuLauout = new QVBoxLayout;

    QString names[3] = {"H", "S", "V"};
    int vals[3] = {360, 255, 255};

    _comboBox->addItem("RED");
    _comboBox->addItem("BLUE");
    _comboBox->addItem("YELLOW");
    menuLauout->addWidget(_comboBox);

    for (int i = 0; i < 3; i++){
        QHBoxLayout* l = new QHBoxLayout;
        QLabel* name = new QLabel(this);
        name->setText(names[i] + ": ");

        _sliders[i]= new ctkRangeSlider(this);
        _sliders[i]->setOrientation(Qt::Orientation::Horizontal);
        _sliders[i]->setMaximum(vals[i]);

        l->addWidget(name);
        l->addWidget(_sliders[i]);
        menuLauout->addLayout(l);
    }

    _pipetBtn->setCheckable(true);
    _pipetBtn->setText("Пипетка");
    menuLauout->addWidget(_pipetBtn);

    _saveBtn->setText("Сохранить");

    layout->addWidget(_image);
    menuLauout->addWidget(_saveBtn);
    layout->addLayout(menuLauout);
    setLayout(layout);
}

void ConfigWidget::_initConnections()
{
    connect(_image, &ClickableLabel::clicked, this, &ConfigWidget::_setIntervals);
    connect(_saveBtn, &QPushButton::clicked, this, &ConfigWidget::_saveConfig);
    connect(_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int){_loadSettings();});
}

void ConfigWidget::_loadSettings()
{
    _settings->beginGroup(_comboBox->currentText());
    _sliders[0]->setMaximumValue(_settings->value("h_high", 360).toInt());
    _sliders[0]->setMinimumValue(_settings->value("h_low", 0).toInt());
    _sliders[1]->setMaximumValue(_settings->value("s_high", 255).toInt());
    _sliders[1]->setMinimumValue(_settings->value("s_low", 0).toInt());
    _sliders[2]->setMaximumValue(_settings->value("v_high", 255).toInt());
    _sliders[2]->setMinimumValue(_settings->value("v_low", 0).toInt());
    _settings->endGroup();
}

void ConfigWidget::_saveConfig()
{
    _settings->beginGroup(_comboBox->currentText());
    _settings->setValue("h_high", _sliders[0]->maximumValue());
    _settings->setValue("h_low", _sliders[0]->minimumValue());
    _settings->setValue("s_high", _sliders[1]->maximumValue());
    _settings->setValue("s_low", _sliders[1]->minimumValue());
    _settings->setValue("v_high", _sliders[2]->maximumValue());
    _settings->setValue("v_low", _sliders[2]->minimumValue());
    _settings->endGroup();
}

void ConfigWidget::_setIntervals(int h, int s, int v)
{
    if (!_pipetBtn->isChecked()) return;
    _pipetBtn->setChecked(false);
    if (h < 25) h += 180;
    _sliders[0]->setMinimumValue(MAX(h - 25, 0));
    _sliders[0]->setMaximumValue(MIN(h + 25, 180));
    _sliders[1]->setMinimumValue(MAX(s - 25, 0));
    _sliders[1]->setMaximumValue(MIN(s + 25, 255));
    _sliders[2]->setMinimumValue(MAX(v - 50, 0));
    _sliders[2]->setMaximumValue(MIN(v + 50, 255));
}

void ConfigWidget::UpdateCamera(QPixmap pixmap)
{
    *_pixmap = pixmap;
    QImage imag = pixmap.copy(pixmap.rect()).toImage();
    cv::Mat black = cv::Mat::zeros(imag.height(),imag.width(),CV_8UC4);
    cv::Mat img(imag.height(),imag.width(),CV_8UC4,
                static_cast<void*>(const_cast<uchar *>(imag.constBits())),
                static_cast<size_t>(imag.bytesPerLine()));

    int hMin = _sliders[0]->minimumValue();
    int hMax = _sliders[0]->maximumValue();
    int sMin = _sliders[1]->minimumValue();
    int sMax = _sliders[1]->maximumValue();
    int vMin = _sliders[2]->minimumValue();
    int vMax = _sliders[2]->maximumValue();

    if (hMin > 180){
        hMin -= 180;
        hMax -= 180;
    }
    cv::Mat image = img.clone();
    cv::cvtColor(image, image, CV_BGR2HSV);
    cv::Scalar upper(MIN(hMax, 180), sMax, vMax);
    cv::Scalar lower(hMin, sMin, vMin);
    cv::inRange(image, lower, upper, image);

    cv::Mat image1 = img.clone();
    cv::cvtColor(image1, image1, CV_BGR2HSV);
    cv::Scalar upper1(MAX(hMax - 180, 0), sMax, vMax);
    cv::Scalar lower1(0, sMin, vMin);
    cv::inRange(image1, lower1, upper1, image1);

    image = image + image1;
    cv::subtract(black, img, img, 255 - image);

    cv::cvtColor(img, img, CV_BGR2RGB);
//    cv::imshow("test", img);
//    cv::waitKey(1);
    QImage dest((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    _image->setPixmap(QPixmap::fromImage(dest));
//    _image->setPixmap(*_pixmap);
}
