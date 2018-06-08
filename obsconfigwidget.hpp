#ifndef OBSCONFIGWIDGET_HPP
#define OBSCONFIGWIDGET_HPP

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QWebSocket>
#include <QLineEdit>
#include <QWebSocket>
#include <QComboBox>

class ObsConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ObsConfigWidget(QWidget *parent = nullptr);

signals:
    void newObsData(QVector<double> data);

private:
    void _createLayout();
    QLayout* _createObsLayout();
    QLayout* _createCameraslayout();
    void _initConnections();
    void _socketConnect();
    void _socketDisconnect();
    void _socketgetBinaryData(const QByteArray &message);
    void _socketerror(QAbstractSocket::SocketError error);
    void _parseData();
    void _camInfoUpdate();
    void _camInfoSend();
    void _sendObs();

    const QString URL= "ws://192.168.1.65:4090";
    QLineEdit* _queryText;
    QTextEdit* _textArea;
    QComboBox* _comboBox;
    QComboBox* _camerasBox;
    QLineEdit* _fps;
    QLineEdit* _quality;
    QWebSocket _socket;
};

#endif // OBSCONFIGWIDGET_HPP
