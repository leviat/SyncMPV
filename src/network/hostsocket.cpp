#include "hostsocket.hpp"

#include <sync/protocol.hpp>

// Qt
#include <QTcpSocket>
#include <QTcpServer>

namespace network {

HostSocket::HostSocket()
{
    status = NONE;
    connect(&server, &QTcpServer::newConnection, this, &HostSocket::addClient);
}

HostSocket::~HostSocket() {
    closeConnection();
}

void HostSocket::openConnection() {
    if (status != CONNECTION_OPEN) {
        if (!server.listen(QHostAddress::Any, 8000)) {
            emit serverError(); // TODO: dont forget to connect this signal
            qDebug() << "Could not open server";
            return;
        }

        status = CONNECTION_OPEN;
    }
}

void HostSocket::addClient() {
    QTcpSocket *clientConnection = server.nextPendingConnection();
    clientConnection->setSocketOption(QTcpSocket::LowDelayOption, 1);
    clients.append(clientConnection);

    connect(clientConnection, &QTcpSocket::disconnected, this, [=]() { clients.removeOne(reinterpret_cast<QTcpSocket*>(sender())); });
    connect(clientConnection, &QTcpSocket::disconnected, clientConnection, &QObject::deleteLater);
    connect(clientConnection, &QTcpSocket::readyRead, this, [=]() { emit newClient(reinterpret_cast<QTcpSocket*>(sender())); });
}

void HostSocket::broadcast(QByteArray& packet) {
    for (QTcpSocket* client : clients) {
        if (client->state() == QTcpSocket::ConnectedState)
            client->write(packet);
    }
}



void HostSocket::closeConnection() {
    if (status == CONNECTION_OPEN) {
        server.close();
        status = CONNECTION_CLOSED;
    }
}

}
