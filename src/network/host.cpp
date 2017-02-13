#include "host.hpp"

#include <network/protocol.hpp>

// Qt
#include <QTcpSocket>
#include <QTcpServer>

namespace network {

Host::Host()
{
    status = NONE;
    connect(&server, &QTcpServer::newConnection, this, &Host::addClient);
}

void Host::openConnection() {
    if (status != CONNECTION_OPEN) {
        if (!server.listen(QHostAddress::Any, 8000)) {
                emit serverError(); // TODO: dont forget to connect this signal
                return;
        }

        status = CONNECTION_OPEN;
    }
}

void Host::addClient() {

    QTcpSocket *clientConnection = server.nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);

    connect(clientConnection, &QTcpSocket::readyRead, this, &Host::checkClientName);
}

void Host::checkClientName() {
    QObject* obj = sender();
    disconnect(reinterpret_cast<QIODevice*>(obj), &QTcpSocket::readyRead, this, &Host::checkClientName);

    QByteArray packet = reinterpret_cast<QTcpSocket*>(obj)->readAll();
    qDebug() << Protocol::toName(packet);
}

void Host::closeConnection() {
    if (status == CONNECTION_OPEN) {
        server.close();
        status = CONNECTION_CLOSED;
    }
}

}
