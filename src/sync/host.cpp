#include "host.hpp"

#include <mplayer/mpvobject.hpp>
#include <sync/protocol.hpp>
#include <sync/clientinfomodel.hpp>
#include <network/hostsocket.hpp>

//Qt
#include <QTcpSocket>
#include <QQmlListProperty>

namespace sync {


Host::Host(QObject *parent) : QObject (parent)
{
    QObject::connect(&m_socket, SIGNAL(newClient(QTcpSocket*)), this, SLOT(addClient(QTcpSocket*)));
}

void Host::openConnection() {
    m_socket.openConnection();
}

void Host::closeConnection() {
    m_socket.closeConnection();
}

void Host::broadcastPlayerState() {
    mplayer::MpvObject* mpv = reinterpret_cast<mplayer::MpvObject*>(sender());
    mplayer::state playerState = mpv->state();
    QByteArray packet;
    sync::Protocol::toSyncPacket(packet, playerState);
    m_socket.broadcast(packet);
}

void Host::processPackage() {
    QTcpSocket* client = reinterpret_cast<QTcpSocket*>(sender());
    QByteArray packetBytes = client->readAll();
    Protocol::Packet packet = Protocol::toPacket(packetBytes);

    if (packet.phase == Protocol::INIT) {
        QString name = Protocol::toName(packet.data);
        for ( auto clientInfo : m_clients) {
            if( clientInfo->address() == client->peerAddress()) {
                clientInfo->name() = name;
            }
        }
    }

    if (packet.phase == Protocol::SYNC) {
        //mplayer::state playerState = Protocol::toPlayerState(packet.data);
    }

}

void Host::addClient(QTcpSocket* client) {
    ClientInfo* clientInfo = new ClientInfo();
    clientInfo->setAddress(client->peerAddress());
    clientInfo->setPort(client->peerPort());
    qDebug() << clientInfo->address();
    QObject::connect(client, &QTcpSocket::readyRead, this, &Host::processPackage);
    m_clients.append(clientInfo);
    QObject::connect(client, &QTcpSocket::disconnected, this, [=]() { removeClient(client->peerAddress()) ;});
}

void Host::removeClient(QHostAddress address) {
    ClientInfo* clientToRemove = nullptr;

    for( auto clientInfo : m_clients) {
        if (clientInfo->address() == address)
            clientToRemove = clientInfo;
    }

    m_clients.removeOne(clientToRemove);
}

} // namespace Sync
