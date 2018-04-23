#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include <QObject>

#include "rov_data_types/rov_data_types.hpp"
#include "tcpconnetor.hpp"

class DataStore : public QObject
{
    Q_OBJECT
public:
    explicit DataStore(QWidget *parent = nullptr);

signals:
    void telimetryUpdate(float yaw, float pitch, float roll);
    void tcpConnect();
    void tcpDisconnect();

private:
    void _initConnections();
    void _getPackage(const std::vector<std::uint8_t> &package);

    TcpConnector _connector;
    rov_types::rov_telimetry _telimetry;
    rov_types::rov_control _control;
    rov_types::rov_hardware_firmware _hardware_firmware;
};

#endif // DATASTORE_HPP
