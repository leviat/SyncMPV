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

void Host::processPackage(QByteArray &packet) {
    //TODO
}

void Host::addClient(QTcpSocket* client) {
    qDebug() << client->peerAddress();
    //disconnect(client, &QTcpSocket::readyRead, this, &network::HostSocket::checkClientName);

    //QByteArray packet = client->readAll();
    //qDebug() << sync::Protocol::toName(packet);

}

} // namespace Sync
