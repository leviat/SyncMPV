#ifndef HOST_HPP
#define HOST_HPP

#include <QObject>
#include <network/hostsocket.hpp>

namespace sync {

class Host : public QObject
{
    Q_OBJECT

private:
    network::HostSocket socket;

public:
    Host(QObject *parent = 0);

public slots:
    void broadcastPlayerState();
    void processPackage(QByteArray& packet);
    void addClient(QTcpSocket* client);
    void openConnection();
    void closeConnection();

};

} // namespace sync
#endif // HOST_HPP
