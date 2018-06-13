#include "obsconfigwidget.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QIntValidator>
#include <QJsonArray>

ObsConfigWidget::ObsConfigWidget(QWidget *parent) : QWidget(parent)
{
    _createLayout();
    _initConnections();
//    _socket.open(QUrl(URL));
}

void ObsConfigWidget::_createLayout()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QLayout* obsLayout = _createObsLayout();
    QLayout* camLayout = _createCameraslayout();

    mainLayout->addLayout(obsLayout);
    mainLayout->addLayout(camLayout);
    this->setLayout(mainLayout);
}

QLayout *ObsConfigWidget::_createObsLayout()
{
    QVBoxLayout* obslayout = new QVBoxLayout;
    QHBoxLayout* queryLayout = new QHBoxLayout;
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    QHBoxLayout* oneMoreButtonsLayout = new QHBoxLayout;

    _textArea = new  QTextEdit(this);
//    _textArea->setEnabled(false);

    auto lable = new QLabel(this);
    lable->setText("Request");
    _queryText = new QLineEdit(this);
    _queryText->setEnabled(false);

    auto ws = new QPushButton(this);
    ws->setText("Connect ws");
    auto parse = new QPushButton(this);
    parse->setText("Draw chart");
    auto sendBtn = new QPushButton(this);
    sendBtn->setText("Send");
    auto search = new QPushButton(this);
    search->setText("Find AP");
    auto connect1 = new QPushButton(this);
    connect1->setText("Connect AP");
    auto connect2 = new QPushButton(this);
    connect2->setText("Connect server");
    auto esp = new QPushButton(this);
    esp->setText("Esp info");
    auto http = new QPushButton(this);
    http->setText("Http request");
    auto disconnect = new QPushButton(this);
    disconnect->setText("Disconnect");

    connect(sendBtn, &QPushButton::pressed, this,&ObsConfigWidget::_sendObs);
    connect(parse, &QPushButton::pressed, this,&ObsConfigWidget::_parseData);
    connect(ws, &QPushButton::pressed, [this](){_socket.open(QUrl(URL));});
    connect(search, &QPushButton::pressed, [this](){_queryText->setText("AT+CWLAP=\"MATE2018\"");});
    connect(connect1, &QPushButton::pressed, [this](){_queryText->setText("AT+CWJAP=\"MATE2018\",\"\"");});
    connect(connect2, &QPushButton::pressed, [this](){_queryText->setText("AT+CIPSTART=\"TCP\",\"192.168.4.1\",80");});
    connect(esp, &QPushButton::pressed, [this](){_queryText->setText("AT+CIPSEND=16");});
    connect(http, &QPushButton::pressed, [this](){_queryText->setText("GET / HTTP/1.1\r\n");});
    connect(disconnect, &QPushButton::pressed, [this](){_queryText->setText("AT+CWQAP");});


    queryLayout->addWidget(lable);
    queryLayout->addWidget(_queryText);
    buttonsLayout->addWidget(search);
    buttonsLayout->addWidget(connect1);
    buttonsLayout->addWidget(connect2);
    buttonsLayout->addWidget(esp);
    buttonsLayout->addWidget(http);
    buttonsLayout->addWidget(disconnect);
    buttonsLayout->addWidget(sendBtn);
    oneMoreButtonsLayout->addWidget(ws);
    oneMoreButtonsLayout->addWidget(parse);

    obslayout->addLayout(oneMoreButtonsLayout);
    obslayout->addWidget(_textArea);
    obslayout->addLayout(queryLayout);
    obslayout->addLayout(buttonsLayout);

    return obslayout;
}

QLayout *ObsConfigWidget::_createCameraslayout()
{
    QVBoxLayout* camlayout = new QVBoxLayout;
    QHBoxLayout* sizeLayout = new QHBoxLayout;
    QHBoxLayout* qualityLayout = new QHBoxLayout;
    QHBoxLayout* fpsLayout = new QHBoxLayout;
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    QHBoxLayout* camSelectLayout = new QHBoxLayout;

    auto lablesize = new QLabel(this);
    lablesize->setText("Size");
    _comboBox = new QComboBox(this);
    _comboBox->addItems({"352 x 240", "640 x 360", "800 x 600", "854 x 480", "960 x 540", "1024 x 768", "1152 x 864"});

    sizeLayout->addWidget(lablesize);
    sizeLayout->addWidget(_comboBox);

    auto lableQuality = new QLabel(this);
    lableQuality->setText("Quality");
    _quality = new QLineEdit(this);
    _quality->setValidator(new QIntValidator(1, 100));

    qualityLayout->addWidget(lableQuality);
    qualityLayout->addWidget(_quality);

    auto lablefps = new QLabel(this);
    lablefps->setText("FPS");
    _fps = new QLineEdit(this);
    _fps->setValidator(new QIntValidator(1, 10000));

    fpsLayout->addWidget(lablefps);
    fpsLayout->addWidget(_fps);

    auto lableSelect = new QLabel(this);
    lableSelect->setText("Select camera");
    _camerasBox = new QComboBox(this);

    camSelectLayout->addWidget(lableSelect);
    camSelectLayout->addWidget(_camerasBox);

    auto saveBtn = new QPushButton(this);
    saveBtn->setText("Save");
    connect(saveBtn, &QPushButton::pressed, this,&ObsConfigWidget::_camInfoSend);
    auto refreshBtn = new QPushButton(this);
    refreshBtn->setText("Update");
    connect(refreshBtn, &QPushButton::pressed, this,&ObsConfigWidget::_camInfoUpdate);

    buttonsLayout->addWidget(saveBtn);
    buttonsLayout->addWidget(refreshBtn);

    camlayout->addLayout(sizeLayout);
    camlayout->addLayout(qualityLayout);
    camlayout->addLayout(fpsLayout);
    camlayout->addLayout(camSelectLayout);
    camlayout->addLayout(buttonsLayout);

    camlayout->setSizeConstraint(QLayout::SetMinimumSize);
    return camlayout;
}

void ObsConfigWidget::_initConnections()
{
    connect(&_socket, &QWebSocket::connected, this, &ObsConfigWidget::_socketConnect);
    connect(&_socket, &QWebSocket::disconnected, this, &ObsConfigWidget::_socketDisconnect);
    connect(&_socket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &ObsConfigWidget::_socketerror);
    connect(&_socket, &QWebSocket::binaryMessageReceived, this, &ObsConfigWidget::_socketgetBinaryData);
}

void ObsConfigWidget::_socketConnect()
{
    _queryText->setEnabled(true);
    _camInfoUpdate();
}

void ObsConfigWidget::_socketDisconnect()
{
    _queryText->setEnabled(false);
}

void ObsConfigWidget::_socketgetBinaryData(const QByteArray &message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message);
    qDebug() << "bin " << doc["type"].toString() << " Size: " << message.size();

    if (doc["type"].toString() == "OBS")
        _textArea->setText(_textArea->toPlainText() + doc["payload"].toString());
    else if (doc["type"].toString() == "cameras_config_reply"){
        _camerasBox->clear();
        for (auto i : doc["cameras"].toArray())
            _camerasBox->addItem(QString::number(i.toInt()));

        _camerasBox->setCurrentText(QString::number(doc["camera"].toInt()));
        _fps->setText(QString::number(doc["fps"].toInt()));
        _quality->setText(QString::number(doc["quality"].toInt()));
        _comboBox->setCurrentText(QString::number(doc["width"].toInt()) + " x " + QString::number(doc["height"].toInt()));
    }
}

void ObsConfigWidget::_socketerror(QAbstractSocket::SocketError error)
{
    _queryText->setEnabled(false);
}

void ObsConfigWidget::_parseData()
{
//    QString str = "<h1>MATE 2018 SEISMOGRAPH STATUS</h1>Voltage=4.966 Xangle=-0.128 Yangle=0.005 Count=2</p>DATA: 0, 4.4, -4.3, 0, 0, 6.8, -7, 0, 0, 1.6, -1.6, 0, 0, 6.3, -6.4, 0";

//    QString REG = "DATA: ";
//    for (int i = 0; i < 16; i++)
//        REG += "([-+]*\\d+(?:\\.\\d+)*).*?";
//    qDebug() << REG;
//    QRegExp rx(REG);

//    QVector<double> data;;
//    int pos = 0;

//    qDebug() << rx.indexIn(str, pos);
//    if (rx.indexIn(str, pos) == -1) return;
//    for (int i = 1; i < 17; i++)
//        data.push_back(rx.cap(i).toDouble());

//    emit newObsData(data);

    QString str = _textArea->toPlainText();
    QVector<double> data;

    auto pos = str.indexOf("DATA: ");
    if (pos == -1) return;

    for (auto i : str.right(str.size() - pos).split(", "))
        data.push_back(i.toDouble());

    emit newObsData(data);
}

void ObsConfigWidget::_camInfoUpdate()
{
    if (!_queryText->isEnabled()) return;

    QJsonObject json;
    json["type"] = "camera_get_config";

    QJsonDocument jsonDoc(json);
    _socket.sendTextMessage(jsonDoc.toJson(QJsonDocument::Compact));
}

void ObsConfigWidget::_camInfoSend()
{
    if (!_queryText->isEnabled()) return;

    QJsonObject json;
    json["type"] = "camera_set_config";
    auto strings = _comboBox->currentText().split(" x ");
    json["width"] = strings[0].toInt();
    json["height"] = strings[1].toInt();
    json["fps"] = _fps->text().toInt();
    json["quality"] = _quality->text().toInt();
    json["camera"] = _camerasBox->currentText().toInt();


    QJsonDocument jsonDoc(json);
    _socket.sendTextMessage(jsonDoc.toJson(QJsonDocument::Compact));
}

void ObsConfigWidget::_sendObs()
{
    if (!_queryText->isEnabled()) return;

    QJsonObject json;
    json["type"] = "OBS";
    json["payload"] = _queryText->text();

    QJsonDocument jsonDoc(json);
    _socket.sendTextMessage(jsonDoc.toJson(QJsonDocument::Compact));

}
