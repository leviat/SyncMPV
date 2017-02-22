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
    m_clientInfoModel = new ClientInfoModel();
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
        m_clientInfoModel->setName(client->peerAddress(), name);
    }

    if (packet.phase == Protocol::SYNC) {
        //mplayer::state playerState = Protocol::toPlayerState(packet.data);
    }

}

void Host::addClient(QTcpSocket* client) {
    ClientInfo* clientInfo = new ClientInfo();
    clientInfo->setAddress(client->peerAddress());
    clientInfo->setPort(client->peerPort());
    m_clientInfoModel->addClientInfo(clientInfo);

    qDebug() << clientInfo->address();
    QObject::connect(client, &QTcpSocket::readyRead, this, &Host::processPackage);
    QObject::connect(client, &QTcpSocket::disconnected, this, [=]() { removeClient(client->peerAddress()) ;});
}

void Host::removeClient(QHostAddress address) {
    m_clientInfoModel->removeClientInfo(address);
}

void Host::setClientInfoModel(ClientInfoModel* clientInfoModel) {
    if (clientInfoModel != m_clientInfoModel) {
        delete m_clientInfoModel;
        m_clientInfoModel = clientInfoModel;
    }
}

void Host::setMpv(mplayer::MpvObject *mpv_instance) {
    m_mpv = mpv_instance;
}

} // namespace Sync
