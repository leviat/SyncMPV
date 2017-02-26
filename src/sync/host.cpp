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
    m_clientInfoModel = new ClientInfoModel;
    QObject::connect(&m_socket, SIGNAL(newClient(QTcpSocket*)), this, SLOT(addClient(QTcpSocket*)));
    qDebug() << "boop";
}

void Host::openConnection() {
    m_socket.openConnection();
}

void Host::closeConnection() {
    m_socket.closeConnection();
}

void Host::broadcastPlayerState(mplayer::state state) {
    QByteArray packet;
    sync::Protocol::toSyncPacket(packet, state);
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
        mplayer::state playerState = Protocol::toPlayerState(packet.data);

        quint16 bufferProgress = 0;

        // in percent = (currentBuffer in s) / remaining media duration
        // demuxer buffer / remainingDuration in s + addBuffer / (remainingDuration in percent * file size)
        quint16 playProgress = 0; // currentDuration / fullduration in s

        if (m_mediumInfo.duration == 0 || m_mediumInfo.fileSize == 0) {
            bufferProgress = 100;
            playProgress = 100;
        }
        else {
            double remainingkB = (1 - playerState.playTime / m_mediumInfo.duration) * m_mediumInfo.fileSize;
            double progress = playerState.demuxerCache / (m_mediumInfo.duration - playerState.playTime); //demuxer
            progress += playerState.additionalCache / remainingkB;
            bufferProgress = progress * 100;
            playProgress = playerState.playTime / m_mediumInfo.duration * 100;
        }

        m_clientInfoModel->setBufferProgress(client->peerAddress(), bufferProgress);
        m_clientInfoModel->setPlayProgress(client->peerAddress(), playProgress);

        if (playerState.playState == mplayer::BUFFERING) {
            // broadcast pause and wait until everyone has sufficient buffer
        }
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
    if (m_clientInfoModel != nullptr && clientInfoModel != m_clientInfoModel) {
        delete m_clientInfoModel;
        m_clientInfoModel = clientInfoModel;
    }
}

void Host::setMediumInfo(mplayer::mediumInfo mediumInfo) {
    m_mediumInfo = mediumInfo;
}

void Host::setMpv(mplayer::MpvObject *mpv_instance) {
    m_mpv = mpv_instance;
    QObject::connect(m_mpv, &mplayer::MpvObject::stateChanged, this, &sync::Host::broadcastPlayerState);
    QObject::connect(m_mpv, &mplayer::MpvObject::mediumChanged, this, &sync::Host::setMediumInfo);

}

} // namespace Sync
