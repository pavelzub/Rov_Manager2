#ifndef CALCULATIONWIDGET_HPP
#define CALCULATIONWIDGET_HPP

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>
#include <QString>
#include <QLabel>

#include "map.hpp"

class CalculationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CalculationWidget(QWidget *parent = nullptr);

private:
    void _createLayouts();
    void _initConnections();
    QLayout* _createBlock(QString name, QString firstFieldName, QWidget* firstFieldVal, QString secondFieldName, QWidget* secondFieldVal);
    void _calculate();
    bool _integrate(QString foo, double b, double &result);

    QLineEdit* _startUpSpeed;
    QLineEdit* _startHorSpeed;
    QLineEdit* _fallUpSpeed;
    QLineEdit* _fallHorSpeed;
    QLineEdit* _windSpeed;
    QLineEdit* _windAngle;
    QLineEdit* _startAngle;
    QLineEdit* _time;
    QLineEdit* _size;
    QPushButton* _calculateBtn;
    QLabel* _resultAngle;
    QLabel* _resultLen;
    QLine _resultLine;
    Map* _map;

};

#endif // CALCULATIONWIDGET_HPP
