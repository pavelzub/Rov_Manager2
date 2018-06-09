#include "moshnyiwidget.hpp"
#include <QDoubleValidator>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>

MoshnyiWidget::MoshnyiWidget(QWidget *parent) : QWidget(parent)
{
    _createLayout();
}

void MoshnyiWidget::_createLayout()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* HLayout = new QHBoxLayout();
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

    QLineEdit *N, *q, *D, *V, *Cp;

    addWidget("Rotors count:", N, true, "4");
    addWidget("Seawater q:", q, true, "1,025");
    addWidget("Diametr:", D, true, "16");
    addWidget("Speed:", V, true, "4,1");
    addWidget("Efficiency of Turbines:", Cp, true, "0,32");

    auto res = new QLabel(this);
    auto calc = new QPushButton(this);
    calc->setText("Calculate");

    connect(calc, &QPushButton::pressed, [N, q, D, V, Cp, res](){
        double v =  V->text().replace(",", ".").toDouble() * 2.1 / 4.1;
        double V3 = v * v * v;
        double A = D->text().replace(",", ".").toDouble() * D->text().replace(",", ".").toDouble() / 4.0 * M_PI;
        res->setText(QString::number(N->text().replace(",", ".").toDouble() * 0.5 * q->text().replace(",", ".").toDouble() *
                                     A * V3 * Cp->text().replace(",", ".").toDouble() / 1000.0) + " MW");
    });

    HLayout->addLayout(nameLayout);
    HLayout->addLayout(valLayout);
    mainLayout->addLayout(HLayout);
    mainLayout->addWidget(res);
    mainLayout->addWidget(calc);
    setLayout(mainLayout);
}
