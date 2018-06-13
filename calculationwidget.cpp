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
    _calculateBtn(new QPushButton(this))
{
    _createLayouts();
    _initConnections();
}

void CalculationWidget::_createLayouts()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* HLayout = new QHBoxLayout();
    QHBoxLayout* ResLayout = new QHBoxLayout();
    QVBoxLayout* nameLayout = new QVBoxLayout;
    QVBoxLayout* valLayout = new QVBoxLayout;

    auto addWidget = [this, nameLayout, valLayout](QString name, QLineEdit*& widget, bool valid, QString def){
        QLabel* l = new QLabel(this);
        l->setText(name);
        nameLayout->addWidget(l);

        widget = new QLineEdit(this);
        widget->setText(def);
        if (valid) widget->setValidator(new QDoubleValidator(0, 100000, 400, this));
        valLayout->addWidget(widget);
    };

    addWidget("Heading:", _startAngle, true, "182");
    addWidget("Airspeed ascent:", _startHorSpeed, true, "138");
    addWidget("Ascent rate:", _startUpSpeed, true, "11,3");
    addWidget("Time until engine failure:", _time, true, "29");
    addWidget("Airspeed descent:", _fallHorSpeed, true, "98");
    addWidget("Descent rate:", _fallUpSpeed, true, "7,8");
    addWidget("Wind direction:", _windAngle, true, "248");
    addWidget("Wind speed:", _windSpeed, false, "17,2");
    addWidget("Маштаб:", _size, true, "1428");
//    _windSpeed->setText("-(1/270)*t**2+25");

    _map = new Map(this);

    _resultAngle = new QLabel(this);
    _resultLen = new QLabel(this);
    _resultAngle->setFixedHeight(20);
    _resultLen->setFixedHeight(20);
    _calculateBtn->setText("Calculate");

    ResLayout->addWidget(_resultAngle);
    ResLayout->addWidget(_resultLen);
    HLayout->addLayout(nameLayout);
    HLayout->addLayout(valLayout);
    mainLayout->addLayout(HLayout);
    mainLayout->addWidget(_map);
    mainLayout->addLayout(ResLayout);
    mainLayout->addWidget(_calculateBtn);
    setLayout(mainLayout);
}

void CalculationWidget::_initConnections()
{
    connect(_calculateBtn, &QPushButton::released, this, &CalculationWidget::_calculate);
}

void CalculationWidget::_calculate()
{
    if (!_startHorSpeed->text().size() || !_startUpSpeed->text().size()
            || !_fallHorSpeed->text().size() || !_fallUpSpeed->text().size()
            || !_startAngle->text().size() || !_windAngle->text().size()
            || !_time->text().size() || !_windSpeed->text().size()
            || !_size->text().size()){
        _calculateBtn->setStyleSheet("background-color: red");
        return;
    }
    _calculateBtn->setStyleSheet("background-color: none");
    double V1 = _startHorSpeed->text().replace(",", ".").toDouble();
    double v1 = _startUpSpeed->text().replace(",", ".").toDouble();
    double V2 = _fallHorSpeed->text().replace(",", ".").toDouble();
    double v2 = _fallUpSpeed->text().replace(",", ".").toDouble();
    double startAngle = _startAngle->text().replace(",", ".").toDouble();
    double windAnlge = _windAngle->text().replace(",", ".").toDouble() - 180;
    double t = _time->text().replace(",", ".").toDouble();
    double m = _size->text().replace(",", ".").toDouble();
    QString speed = _windSpeed->text().replace(",", ".");

    double fallTime = t * v1 / v2;
    double wind;
    if (!_integrate(speed, fallTime, wind)){
        _resultAngle->setText("Неплохо было бы поставить питон и sympy");
        return;
    }

    QLineF windVec(0, 0, wind, 0);
    QLineF ascentVec(0, 0, V1 * t, 0);
    QLineF fallVec(0, 0, V2 * fallTime, 0);

    windVec.setAngle(windAnlge);
    ascentVec.setAngle(startAngle);
    fallVec.setAngle(startAngle);

    QLineF res(0, 0, windVec.x2() + ascentVec.x2() + fallVec.x2(), windVec.y2() + ascentVec.y2() + fallVec.y2());

    qDebug() << "Wind west: " << windVec.length() << " south:" << windVec.y2();
    qDebug() << "Ascent west: " << ascentVec.length() << " south:" << ascentVec.y2();
    qDebug() << "Fall west: " << fallVec.length() << " south:" << fallVec.y2();
    _resultAngle->setText("Angle: " + QString::number(res.angle()));
    _resultLen->setText("Length: " + QString::number(res.length()));
    _map->setLine(res);
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

