#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <network/clientsocket.hpp>

#include <QObject>

namespace sync {

class Client : public QObject
{
    Q_OBJECT

private:
    network::ClientSocket socket;


public:
    explicit Client(QObject *parent = 0);
    void sendName();
    void processSyncPacket();

signals:

public slots:
    void connect();
    void disconnect();

};

} // namespace sync

#endif // CLIENT_HPP
