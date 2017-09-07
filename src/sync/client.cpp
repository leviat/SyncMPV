#include "client.hpp"

#include <sync/protocol.hpp>

#include <QMetaObject>
#include <QHostAddress>

namespace sync {

/*!
    \class sync::Client
    \inmodule sync
    \brief The client synchronizes its player state with a host.
*/

/*!
 * \brief Constructs a Client.
 */
Client::Client(QObject *parent) : QObject(parent)
{
    m_currentState = {mplayer::PAUSE, 0};
    m_hostAddress = "127.0.0.1";
    m_clientPort = 8001;
    m_speedAdjusted = false;
    m_speedIncreased = false;
    m_speed = 1.0;
    m_name = "";
}

/*!
 * \brief Establishes a connection between the host at \a address and \a hostPort and our \a clientPort..
 */
void Client::connect(QString address, quint16 hostPort, quint16 clientPort, QString name){
    m_name = name;

    QObject::connect(&m_socket, &network::ClientSocket::connected, this, &Client::sendName);
    m_socket.connect(address, hostPort, clientPort);
}

/*!
 * \brief Closes the connection to the host.
 */
void Client::disconnect() {
    m_socket.disconnect();
}

/*!
 * \brief Sends the current known name to the host.
 */
void Client::sendName() {
    QObject::disconnect(&m_socket, &network::ClientSocket::connected, this, &Client::sendName);
    QObject::connect(&m_socket, &network::ClientSocket::newData, this, &Client::processPackage);
    QObject::connect(m_mpv, &mplayer::MpvObject::stateChanged, this, &sync::Client::sendPlayerState);

    QByteArray buffer;
    sync::Protocol::toInitPacket(buffer, m_name);
    m_socket.send(buffer);
}

/*!
 * \brief Processes an incoming package. Only call this from the client's \class ClientSocket.
 */
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

/*!
 * \brief Syncs the player's Play/Pause state with the host's \a playerState.
 */
void Client::adjustPlayState(mplayer::state playerState) {
    if (playerState.playState != mplayer::PLAY && m_currentState.playState == mplayer::PLAY) {
        emit propertyChange("pause", true);
    }
    else if (playerState.playState == mplayer::PLAY
                && m_currentState.playState != mplayer::PLAY
                && m_mediumInfo.fileSize != 0) {
        emit propertyChange("pause", false);
    }
}

/*!
 * \brief Adjusts the player's speed to keep in sync with the host's \a playerState.
 */
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

/*!
 * \brief Updates the information about the played medium with the \a mediumInfo.
 */
void Client::setMediumInfo(mplayer::mediumInfo mediumInfo) {
    m_mediumInfo = mediumInfo;
}

/*!
 * \brief Sets the used mpv instance to \a mpvInstance.
 */
void Client::setMpv(mplayer::MpvObject *mpvInstance) {
    m_mpv = mpvInstance;

    QObject::connect(this, &Client::propertyChange, mpvInstance, &mplayer::MpvObject::setProperty);
    QObject::connect(this, &Client::command, mpvInstance, &mplayer::MpvObject::command);
    QObject::connect(mpvInstance, &mplayer::MpvObject::stateChanged, this, &Client::setState);
    QObject::connect(m_mpv, &mplayer::MpvObject::mediumChanged, this, &Client::setMediumInfo);

}

/*!
 * \brief Sets the current player state to \a state..
 */
void Client::setState(mplayer::state state) {
    m_currentState = state;
}

/*!
 * \brief Send the client's \a state to the host.
 */
void Client::sendPlayerState(mplayer::state state) {
    QByteArray packet;
    sync::Protocol::toSyncPacket(packet, state);
    m_socket.send(packet);
}

} // namespace sync
