#include "client.hpp"

#include <sync/protocol.hpp>

#include <QMetaObject>
#include <QHostAddress>

namespace sync {

Client::Client(QObject *parent) : QObject(parent)
{
    m_currentState = {mplayer::PAUSE, 0};
}

void Client::connect(){
    QObject::connect(&m_socket, &network::ClientSocket::connected, this, &Client::sendName);

    QHostAddress host("127.0.0.1");
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

        if (playerState.playState == mplayer::BUFFERING || playerState.playState == mplayer::PAUSE) {
            emit propertyChange("pause", true);
        }
        else if (playerState.playState == mplayer::PLAY) {
            emit propertyChange("pause", false);
        }

        double timeDiff = playerState.playTime - m_currentState.playTime;

        double speedInterval = 0.20;

        // Only seek for sync when we are totally off
        // Might allow lower seek thresholds for files with fast seeking

        if (std::abs(timeDiff) > 5.0 && m_currentState.demuxerCache > 0.1) {
            QList<QVariant> paramList;
            paramList << QVariant("seek") << QVariant(playerState.playTime) << QVariant("absolute");
            emit command(QVariant(paramList));
        }
        else if (timeDiff > 0.2 ) {
            // we could make this dynamic, however changing the speed frequently is noticable as distorted audio
            // also, setting the correctionspeed too low delays the point where host and client get in sync again
            // for now, we just aggresively get the players in sync asap
            // some hysteresis might be good, though
            double correctionSpeed = 2;
            int fullQuotient = correctionSpeed / speedInterval;
            correctionSpeed = speedInterval * fullQuotient;
            emit propertyChange("speed", correctionSpeed);

        }
        else if (timeDiff < -0.2 ) {
            double correctionSpeed = 0.5;// 1.00 + std::max(timeDiff, -1.0);
            int fullQuotient = correctionSpeed / speedInterval;
            correctionSpeed = speedInterval * fullQuotient;
            emit propertyChange("speed", correctionSpeed);
        }
        else {
            emit propertyChange("speed", 1.0);
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
