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

Host::~Host() {
    closeConnection();
}

void Host::openConnection() {
    if (status != CONNECTION_OPEN) {
        if (!server.listen(QHostAddress::Any, 8000)) {
            emit serverError(); // TODO: dont forget to connect this signal
            qDebug() << "Could not open server";
            return;
        }

        status = CONNECTION_OPEN;
    }
}

void Host::addClient() {
    QTcpSocket *clientConnection = server.nextPendingConnection();
    clientConnection->setSocketOption(QTcpSocket::LowDelayOption, 1);

    connect(clientConnection, &QTcpSocket::disconnected, this, [=]() { clients.removeOne(reinterpret_cast<QTcpSocket*>(sender())); });
    connect(clientConnection, &QTcpSocket::disconnected, clientConnection, &QObject::deleteLater);
    connect(clientConnection, &QTcpSocket::readyRead, this, &Host::checkClientName);
}

void Host::checkClientName() {
    QTcpSocket* client = reinterpret_cast<QTcpSocket*>(sender());
    disconnect(client, &QTcpSocket::readyRead, this, &Host::checkClientName);

    QByteArray packet = client->readAll();
    qDebug() << Protocol::toName(packet);

    clients.append(client);
}


void Host::broadcastPlayerState() {

    mplayer::MpvObject* mpv = reinterpret_cast<mplayer::MpvObject*>(sender());
    mplayer::state playerState = mpv->state();
    QByteArray packet;
    Protocol::toSyncPacket(packet, playerState);

    for (QTcpSocket* client : clients) {
        if (client->state() == QTcpSocket::ConnectedState)
            client->write(packet);
    }
}

void Host::closeConnection() {
    if (status == CONNECTION_OPEN) {
        server.close();
        status = CONNECTION_CLOSED;
    }
}

}
