#include "moshnyiwidget.hpp"
#include <QDoubleValidator>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDebug>

MoshnyiWidget::MoshnyiWidget(QWidget *parent) : QWidget(parent)
{
    _createLayout();
}

void MoshnyiWidget::setDepth(float yaw, float pitch, float roll, float depth)
{
    _depth->setText(QString::number(depth));
}

void MoshnyiWidget::_createLayout()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    mainLayout->addLayout(_createTurboLayout());
    mainLayout->addLayout(_createDepthLayout());

    setLayout(mainLayout);
}

QLayout *MoshnyiWidget::_createTurboLayout()
{
    auto layout = new QVBoxLayout;
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
    addWidget("Speed:", V, true, "4,1");
    addWidget("Diametr:", D, true, "16");
    addWidget("Efficiency of Turbines:", Cp, true, "0,32");
    addWidget("Seawater q:", q, true, "1,025");

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
    layout->addLayout(HLayout);
    layout->addWidget(res);
    layout->addWidget(calc);

    return layout;
}

QLayout *MoshnyiWidget::_createDepthLayout()
{
    auto layout = new QGridLayout;

    _depth = new QLabel(this);
    QLabel* val2 = new QLabel(this);
    QLabel* val3 = new QLabel(this);
    QLabel* val4 = new QLabel(this);

    layout->addWidget(_depth, 0, 1);
    layout->addWidget(val2, 1, 1);
    layout->addWidget(val3, 2, 1);
    layout->addWidget(val4, 3, 1);

    QLabel* depth1 = new QLabel(this);
    depth1->setText("Текущая глубина: ");

    QPushButton* depth2 = new QPushButton(this);
    depth2->setText("Глубина на дне: ");
    connect(depth2, &QPushButton::pressed, [this, val2, val3, val4](){
        val2->setText(_depth->text());
        val4->setText(QString::number(val2->text().toDouble() - val3->text().toDouble() + 2.5));
    });
//    depth2->setFixedSize(100,20);

    QPushButton* depth3 = new QPushButton(this);
    depth3->setText("Глубина установки лага: ");
    connect(depth3, &QPushButton::pressed, [this, val2, val3, val4](){
        val3->setText(_depth->text());
        val4->setText(QString::number(val2->text().toDouble() - val3->text().toDouble() + 2.5));
    });
//    depth3->setFixedSize(100,20);

    QLabel* depth4 = new QLabel(this);
    depth4->setText("Итоговое расстояние: ");

    layout->addWidget(depth1, 0, 0);
    layout->addWidget(depth2, 1, 0);
    layout->addWidget(depth3, 2, 0);
    layout->addWidget(depth4, 3, 0);

    return layout;
}
