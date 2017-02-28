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
}

void Host::openConnection(quint16 port) {
    m_socket.openConnection(port);
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
        m_clientInfoModel->setName(client->peerAddress(), client->peerPort(), name);
    }

    if (packet.phase == Protocol::SYNC) {
        mplayer::state playerState = Protocol::toPlayerState(packet.data);

        quint16 bufferProgress = 0;
        quint16 playProgress = 0;

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

        QString bufferString = QString("buffer: %1s+%2MB").arg((int)playerState.demuxerCache).arg((int)playerState.additionalCache/1024);
        m_clientInfoModel->setBufferProgress(client->peerAddress(), client->peerPort(), bufferProgress);
        m_clientInfoModel->setPlayProgress(client->peerAddress(), client->peerPort(), playProgress);
        m_clientInfoModel->setBufferString(client->peerAddress(), client->peerPort(), bufferString);

        if (playerState.demuxerCache < 2.0) {
            emit waitForBuffer();
            m_waitingForBuffer.insert(client->peerAddress());
        }
        else if (m_waitingForBuffer.remove(client->peerAddress()) && m_waitingForBuffer.empty()) {
            emit bufferIsHealthy();
        }
    }

}

void Host::addClient(QTcpSocket* client) {
    ClientInfo* clientInfo = new ClientInfo();
    clientInfo->setAddress(client->peerAddress());
    clientInfo->setPort(client->peerPort());
    clientInfo->setBufferString("buffer: 0s+0kB");
    m_clientInfoModel->addClientInfo(clientInfo);

    qDebug() << "New client: " << clientInfo->address();
    QObject::connect(client, &QTcpSocket::readyRead, this, &Host::processPackage);
    QObject::connect(client, &QTcpSocket::disconnected, this, [=]() { removeClient(client->peerAddress(), client->peerPort()) ;});
}

void Host::removeClient(QHostAddress address, quint16 port) {
    m_clientInfoModel->removeClientInfo(address, port);
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
    QObject::connect(m_mpv, &mplayer::MpvObject::stateChanged, this, &Host::broadcastPlayerState);
    QObject::connect(m_mpv, &mplayer::MpvObject::mediumChanged, this, &Host::setMediumInfo);
    QObject::connect(this, &Host::waitForBuffer, m_mpv, &mplayer::MpvObject::waitForBuffer);
    QObject::connect(this, &Host::bufferIsHealthy, m_mpv, &mplayer::MpvObject::playAfterBuffering);

}

} // namespace Sync
