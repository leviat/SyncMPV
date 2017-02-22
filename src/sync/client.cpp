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
    emit propertyChange("pause", true);

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

        double timeDiff = std::abs(playerState.playTime - m_currentState.playTime);

        if (timeDiff > 1.0 )
            emit propertyChange("speed", 1.10);
        else if (timeDiff > 0.5)
            emit propertyChange("speed", 1.05);
        else if (timeDiff > 0.2)
            emit propertyChange("speed", 1.02);
        else
            emit propertyChange("speed", 1.00);


    }

}

void Client::setMpv(mplayer::MpvObject *mpvInstance) {
    qDebug() << QObject::connect(this, &sync::Client::propertyChange, mpvInstance, &mplayer::MpvObject::setProperty);
    QObject::connect(mpvInstance, &mplayer::MpvObject::stateChanged, this, &sync::Client::setState);

}

void Client::setState(mplayer::state state) {
    m_currentState = state;
}

} // namespace sync
