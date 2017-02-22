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
    network::HostSocket m_socket;
    QList<ClientInfo*> m_clients; //remove!!!!

public:
    Host(QObject *parent = 0);


signals:

public slots:
    void broadcastPlayerState();
    void processPackage();
    void addClient(QTcpSocket* client);
    void openConnection();
    void closeConnection();
    void removeClient(QHostAddress& address);

};

} // namespace sync
#endif // HOST_HPP
