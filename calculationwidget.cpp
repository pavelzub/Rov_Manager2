#include "calculationwidget.hpp"
#include <QProcess>
#include <QDir>
#include <QDoubleValidator>
#include <iostream>
#include <QtMath>
#include <QLineF>
#include <QClipboard>
#include <QApplication>
#include <Python.h>
#include <QDebug>
#include <QDoubleValidator>

CalculationWidget::CalculationWidget(QWidget *parent) :
    QWidget(parent),
    _startUpSpeed(new QLineEdit(this)),
    _startHorSpeed(new QLineEdit(this)),
    _fallUpSpeed(new QLineEdit(this)),
    _fallHorSpeed(new QLineEdit(this)),
    _windSpeed(new QLineEdit(this)),
    _windAngle(new QLineEdit(this)),
    _startAngle(new QLineEdit(this)),
    _time(new QLineEdit(this)),
    _calculateBtn(new QPushButton(this)),
    _resultAngle(new QLabel(this)),
    _resultLen(new QLabel(this))
{
    _createLayouts();
    _initConnections();
}

void CalculationWidget::_createLayouts()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* Layout_1 = new QHBoxLayout;
    QHBoxLayout* Layout_2 = new QHBoxLayout;
    QHBoxLayout* Layout_3 = new QHBoxLayout;
    QLayout* startLayout = _createBlock("Взлет", "Вертикальная скорость", _startUpSpeed, "Горизонтальная скорость", _startHorSpeed);
    QLayout* fallLayout = _createBlock("Падение", "Вертикальная скорость", _fallUpSpeed, "Горизонтальная скорость", _fallHorSpeed);
    QLayout* initLayout = _createBlock("Данные старта", "Угол", _startAngle, "Время", _time);
    QLayout* windLayout = _createBlock("Ветер", "Угол", _windAngle, "Уравнение скорости", _windSpeed);
    QVBoxLayout* resultLayout  = new QVBoxLayout;

    _startUpSpeed->setValidator(new QDoubleValidator(0, 100000, 400, this));
    _startUpSpeed->setText("10");
    _startHorSpeed->setValidator(new QDoubleValidator(0, 100000, 400, this));
    _startHorSpeed->setText("93");
    _fallUpSpeed->setValidator(new QDoubleValidator(0, 100000, 400, this));
    _fallUpSpeed->setText("6");
    _fallHorSpeed->setValidator(new QDoubleValidator(0, 100000, 400, this));
    _fallHorSpeed->setText("64");
    _windAngle->setValidator(new QDoubleValidator(0, 100000, 400, this));
    _windAngle->setText("270");
    _startAngle->setValidator(new QDoubleValidator(0, 100000, 400, this));
    _startAngle->setText("184");
    _time->setValidator(new QDoubleValidator(0, 100000, 400, this));
    _time->setText("43");
    _windSpeed->setText("-(1/270)*t**2+25");

    _resultAngle->setFixedHeight(75);
    _resultLen->setFixedHeight(75);
    _calculateBtn->setText("Расчитать");
    _resultLen->setStyleSheet("font: 18pt;");
    _resultAngle->setStyleSheet("font: 18pt;");


    _calculateBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    _resultAngle->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    _resultLen->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    Layout_1->addLayout(startLayout);
    Layout_1->addLayout(fallLayout);
    Layout_2->addLayout(windLayout);
    Layout_2->addLayout(initLayout);
    resultLayout->addWidget(_resultAngle);
    resultLayout->addWidget(_resultLen);
    Layout_3->addWidget(_calculateBtn);
    Layout_3->addLayout(resultLayout);
    mainLayout->addLayout(Layout_1);
    mainLayout->addLayout(Layout_2);
    mainLayout->addLayout(Layout_3);
    setLayout(mainLayout);
}

void CalculationWidget::_initConnections()
{
    connect(_calculateBtn, &QPushButton::released, this, &CalculationWidget::_calculate);
}

QLayout *CalculationWidget::_createBlock(QString name, QString firstFieldName, QWidget *firstFieldVal, QString secondFieldName, QWidget *secondFieldVal)
{
    QVBoxLayout* Layout = new QVBoxLayout;
    QLabel* Name = new QLabel;
    QLabel* Lable_1 = new QLabel;
    QLabel* Lable_2 = new QLabel;
    Name->setAlignment(Qt::AlignHCenter);
    Name->setStyleSheet("font: 18pt;");
    Name->setText(name);
    Lable_1->setText(firstFieldName);
    Lable_2->setText(secondFieldName);
    QHBoxLayout* Layout_1 = new QHBoxLayout;
    QHBoxLayout* Layout_2 = new QHBoxLayout;

    Layout->addWidget(Name);
    Layout->addLayout(Layout_1);
    Layout->addLayout(Layout_2);
    Layout_1->addWidget(Lable_1);
    Layout_1->addWidget(firstFieldVal);
    Layout_2->addWidget(Lable_2);
    Layout_2->addWidget(secondFieldVal);

    return Layout;
}

void CalculationWidget::_calculate()
{
    if (!_startHorSpeed->text().size() || !_startUpSpeed->text().size()
            || !_fallHorSpeed->text().size() || !_fallUpSpeed->text().size()
            || !_startAngle->text().size() || !_windAngle->text().size()
            || !_time->text().size() || !_windSpeed->text().size()){
        _resultAngle->setText("Не все поля заполнены");
        _resultLen->clear();
        return;
    }

    double V1 = _startHorSpeed->text().replace(",", ".").toDouble();
    double v1 = _startUpSpeed->text().replace(",", ".").toDouble();
    double V2 = _fallHorSpeed->text().replace(",", ".").toDouble();
    double v2 = _fallUpSpeed->text().replace(",", ".").toDouble();
    double a = _startAngle->text().replace(",", ".").toDouble() / 180.0 * M_PI;
    double b = _windAngle->text().replace(",", ".").toDouble() / 180.0 * M_PI;
    double t = _time->text().replace(",", ".").toDouble();
    QString speed = _windSpeed->text();

    double fallTime = t * v1 / v2;
    double wind;
    if (!_integrate(speed, fallTime, wind)){
        _resultAngle->setText("Неплохо было бы поставить питон и sympy");
        return;
    }

    double windx = -wind * qSin(b);
    double windy = -wind * qCos(b);
    double startx = V1 * t * qSin(a);
    double starty = V1 * t * qCos(a);
    double fallx = V2 * fallTime * qSin(a);
    double fally = V2 * fallTime * qCos(a);

    QLineF res(0, 0, windx + startx + fallx, windy + starty + fally);

    _resultAngle->setText("Angle: " + QString::number(res.angle()));
    _resultLen->setText("Length: " + QString::number(res.length()));
}

bool CalculationWidget::_integrate(QString foo, double b, double &result)
{
    try {
        Py_Initialize();
        PyObject* module = PyImport_Import(PyString_FromString("main"));
        if (!module) return false;
        PyObject* function = PyObject_GetAttrString(module, "foo");
        if (!function) return false;
        PyObject* args = PyTuple_Pack(2, PyString_FromString(foo.toStdString().data()),  PyFloat_FromDouble(b));
        PyObject* ans = PyObject_CallObject(function, args);
        result = PyFloat_AsDouble(ans);
        Py_Finalize();
    } catch (...) {
        qDebug() << "Python error";
        return false;
    }
    return true;
}

