#include "client.hpp"

#include <sync/protocol.hpp>

#include <QMetaObject>
#include <QHostAddress>

namespace sync {

Client::Client(QObject *parent) : QObject(parent)
{
    m_currentState = {mplayer::PAUSE, 0};
    m_hostAddress = "127.0.0.1";
    m_speedAdjusted = false;
    m_speedIncreased = false;
}

void Client::connect(){
    QObject::connect(&m_socket, &network::ClientSocket::connected, this, &Client::sendName);

    QHostAddress host(m_hostAddress);
    m_socket.connect(host);
}

void Client::disconnect() {
    m_socket.disconnect();
}

void Client::sendName() {
    QObject::disconnect(&m_socket, &network::ClientSocket::connected, this, &Client::sendName);
    QObject::connect(&m_socket, &network::ClientSocket::newData, this, &Client::processPackage);
    QObject::connect(m_mpv, &mplayer::MpvObject::stateChanged, this, &sync::Client::sendPlayerState);

    QByteArray buffer;
    sync::Protocol::toInitPacket(buffer, "client_moop");
    m_socket.send(buffer);
}

void Client::processPackage() {
    network::ClientSocket* client = reinterpret_cast<network::ClientSocket*>(sender());
    QByteArray packetBytes = client->readAll();
    Protocol::Packet packet = Protocol::toPacket(packetBytes);

    if (packet.phase == Protocol::SYNC) {
        mplayer::state playerState = Protocol::toPlayerState(packet.data);
        adjustPlayState(playerState);
        adjustSyncSpeed(playerState);
    }

}

void Client::adjustPlayState(mplayer::state playerState) {
    if (playerState.playState != mplayer::PLAY && m_currentState.playState == mplayer::PLAY) {
        emit propertyChange("pause", true);
    }
    else if (playerState.playState == mplayer::PLAY && m_currentState.playState != mplayer::PLAY) {
        emit propertyChange("pause", false);
    }
}

void Client::adjustSyncSpeed(mplayer::state playerState){
    double timeDiff = playerState.playTime - m_currentState.playTime;
    // Only seek for sync when we are totally off
    // Might allow lower seek thresholds for files with fast seeking

    if (std::abs(timeDiff) > 10.0 && m_currentState.demuxerCache > 0.1) {
        QList<QVariant> paramList;
        paramList << QVariant("seek") << QVariant(playerState.playTime) << QVariant("absolute");
        emit command(QVariant(paramList));
    }
    else if (m_speedAdjusted) {
        if ((timeDiff < 0.2 && m_speedIncreased) || (timeDiff > 0.2 && !m_speedIncreased)){
            emit propertyChange("speed", 1.0);
            m_speedAdjusted = false;
        }
    }
    else {
        if (timeDiff > 1.0 ) {
            emit propertyChange("speed", 1.5);
            m_speedAdjusted = true;
            m_speedIncreased = true;
        }
        else if (timeDiff < -1.0 ) {
            emit propertyChange("speed", 0.8);
            m_speedAdjusted = true;
            m_speedIncreased = false;
        }
    }
}

void Client::setMpv(mplayer::MpvObject *mpvInstance) {
    m_mpv = mpvInstance;

    QObject::connect(this, &sync::Client::propertyChange, mpvInstance, &mplayer::MpvObject::setProperty);
    QObject::connect(this, &sync::Client::command, mpvInstance, &mplayer::MpvObject::command);
    QObject::connect(mpvInstance, &mplayer::MpvObject::stateChanged, this, &sync::Client::setState);
}

void Client::setState(mplayer::state state) {
    m_currentState = state;
}

void Client::sendPlayerState(mplayer::state state) {
    QByteArray packet;
    sync::Protocol::toSyncPacket(packet, state);
    m_socket.send(packet);
}

} // namespace sync
