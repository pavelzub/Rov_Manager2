#include "obsconfigwidget.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QIntValidator>

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
    _textArea->setEnabled(false);

    auto lable = new QLabel(this);
    lable->setText("Запрос");
    _queryText = new QLineEdit(this);
    _queryText->setEnabled(false);

    auto ws = new QPushButton(this);
    ws->setText("connect ws");
    auto parse = new QPushButton(this);
    parse->setText("Нарисовать график");
    auto sendBtn = new QPushButton(this);
    sendBtn->setText("Отправить");
    auto search = new QPushButton(this);
    search->setText("поиск AP");
    auto connect1 = new QPushButton(this);
    connect1->setText("connect AP");
    auto connect2 = new QPushButton(this);
    connect2->setText("connect server");
    auto esp = new QPushButton(this);
    esp->setText("huinya dlya esp");
    auto http = new QPushButton(this);
    http->setText("http request");
    auto disconnect = new QPushButton(this);
    disconnect->setText("disconnect");

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
    lablesize->setText("Размер");
    _comboBox = new QComboBox(this);
    _comboBox->addItems({"352 x 240", "640 x 360", "800 x 600", "854 x 480", "960 x 540", "1024 x 768", "1152 x 864"});

    sizeLayout->addWidget(lablesize);
    sizeLayout->addWidget(_comboBox);

    auto lableQuality = new QLabel(this);
    lableQuality->setText("Качество");
    _quality = new QLineEdit(this);
    _quality->setValidator(new QIntValidator(1, 100));

    qualityLayout->addWidget(lableQuality);
    qualityLayout->addWidget(_quality);

    auto lablefps = new QLabel(this);
    lablefps->setText("Фпс");
    _fps = new QLineEdit(this);
    _fps->setValidator(new QIntValidator(1, 10000));

    fpsLayout->addWidget(lablefps);
    fpsLayout->addWidget(_fps);

    auto lableSelect = new QLabel(this);
    lableSelect->setText("Выбор камеры");
    _camerasBox = new QComboBox(this);

    camSelectLayout->addWidget(lableSelect);
    camSelectLayout->addWidget(_camerasBox);

    auto saveBtn = new QPushButton(this);
    saveBtn->setText("Сохранить");
    connect(saveBtn, &QPushButton::pressed, this,&ObsConfigWidget::_camInfoSend);
    auto refreshBtn = new QPushButton(this);
    refreshBtn->setText("Обновить");
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
        _textArea->setText(doc["payload"].toString());
}

void ObsConfigWidget::_socketerror(QAbstractSocket::SocketError error)
{
    _queryText->setEnabled(false);
}

void ObsConfigWidget::_parseData()
{
    emit newObsData({ 0, 4.4, -4.3, 0, 0, 6.8, -7, 0, 0, 1.6, -1.6, 0, 0, 6.3, -6.4, 0});
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
