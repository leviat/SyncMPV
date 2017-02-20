#ifndef HOST_HPP
#define HOST_HPP

#include <network/hostsocket.hpp>
#include <sync/clientinfo.hpp>

#include <memory>

// Qt
#include <QObject>

namespace sync {

class Host : public QObject
{
    Q_OBJECT

private:
    network::HostSocket socket;
    QList<std::shared_ptr<ClientInfo>> clients;

public:
    Host(QObject *parent = 0);

public slots:
    void broadcastPlayerState();
    void processPackage();
    void addClient(QTcpSocket* client);
    void openConnection();
    void closeConnection();

};

} // namespace sync
#endif // HOST_HPP
