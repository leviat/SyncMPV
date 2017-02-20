#include "host.hpp"

#include <mplayer/mpvobject.hpp>
#include <sync/protocol.hpp>
#include <network/hostsocket.hpp>

//Qt
#include <QTcpSocket>

namespace sync {


Host::Host(QObject *parent) : QObject (parent)
{
    QObject::connect(&socket, SIGNAL(newClient(QTcpSocket*)), this, SLOT(addClient(QTcpSocket*)));
}

void Host::openConnection() {
    socket.openConnection();
}

void Host::closeConnection() {
    socket.closeConnection();
}

void Host::broadcastPlayerState() {

    mplayer::MpvObject* mpv = reinterpret_cast<mplayer::MpvObject*>(sender());
    mplayer::state playerState = mpv->state();
    QByteArray packet;
    sync::Protocol::toSyncPacket(packet, playerState);
    socket.broadcast(packet);
}

void Host::processPackage() {
    QTcpSocket* client = reinterpret_cast<QTcpSocket*>(sender());
    QByteArray packetBytes = client->readAll();
    Protocol::Packet packet = Protocol::toPacket(packetBytes);

    if (packet.phase == Protocol::INIT) {
        QString name = Protocol::toName(packet.data);
        for ( auto clientInfo : clients) {
            if( clientInfo->address == client->peerAddress()) {
                clientInfo->name = name;
            }
        }
    }

    if (packet.phase == Protocol::SYNC) {
        //mplayer::state playerState = Protocol::toPlayerState(packet.data);
    }

}

void Host::addClient(QTcpSocket* client) {
    std::shared_ptr<ClientInfo> clientInfo = std::make_shared<ClientInfo>(client->peerAddress(), client->peerPort(), "", 0, 0);
    qDebug() << clientInfo->address;
    QObject::connect(client, &QTcpSocket::readyRead, this, &Host::processPackage);
    clients.append(clientInfo);
}

} // namespace Sync
