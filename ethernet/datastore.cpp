#include "datastore.hpp"
#include <QShortcut>

DataStore::DataStore(QWidget *parent) :
    QObject(parent)
{
    _initConnections();
}

void DataStore::_initConnections()
{
    connect(&_connector, &TcpConnector::dataUpdate, this, &DataStore::_getPackage);
    connect(&_connector, &TcpConnector::Connect, [this](){emit tcpConnect();});
    connect(&_connector, &TcpConnector::Disconnect, [this](){emit tcpDisconnect();});
}

void DataStore::_getPackage(const std::vector<uint8_t> &package)
{
    switch (package[0]) {
        case rov_types::rov_telimetry::meta().packet_id:
            _telimetry.deserialize(package);
            emit(telimetryUpdate(_telimetry.yaw, _telimetry.pitch, _telimetry.roll));
            break;
    }
}
