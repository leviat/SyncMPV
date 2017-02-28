#include "clientsocket.hpp"

#include <sync/protocol.hpp>

// Qt
#include <QTcpSocket>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QNetworkInterface>

namespace network {

/*!
    \class network::ClientSocket
    \inmodule network
    \brief The ClientSocket establishes a connection to a peer (i.e. the host) to send and receive data.
*/

/*!
  \fn void ClientSocket::connected()
  \brief Is emitted when the socket successfully connects to the peer.
*/

/*!
  \fn void ClientSocket::newData()
  \brief Is emitted when the socket received new data from the peer.
*/

/*!
 * \brief Constructs an unconnected ClientSocket.
 */

ClientSocket::ClientSocket()
{
    QObject::connect(&socket, static_cast<void(QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error),
                     [=](QTcpSocket::SocketError socketError){ qDebug() << "Client error: " << socketError; });

    QObject::connect(&socket, &QTcpSocket::stateChanged,  [=](){ qDebug() << "Client state: " << socket.state(); });
    QObject::connect(&socket, &QTcpSocket::connected, this, &ClientSocket::connected);
    QObject::connect(&socket, &QTcpSocket::readyRead, this, &ClientSocket::newData);

}

/*!
 * \brief Destroys the socket.
 */
ClientSocket::~ClientSocket() {
    disconnect();
}

/*!
 * \brief Binds the socket to the \a clientPort and connects it to the \a hostPort at the \a address.
 */
void ClientSocket::connect(QString address, quint16 hostPort, quint16 clientPort) {
    if (socket.state() != QTcpSocket::UnconnectedState)
        return;

    socket.bind(clientPort);
    socket.connectToHost(address, hostPort);
    socket.setSocketOption(QTcpSocket::LowDelayOption, 1);

}

/*!
 * \brief Sends the \a packet to the peer.
 */
void ClientSocket::send(QByteArray& packet) {
    if (socket.state() != QTcpSocket::ConnectedState) {
        qDebug() << "Client tries to send data but is not connected.";
        return;
    }

    socket.write(packet);
}

/*!
 * \brief Returns the available data
 */
QByteArray ClientSocket::readAll() {
    return socket.readAll();
}

/*!
 * \brief Disconnects the socket.
 */
void ClientSocket::disconnect() {
    if (socket.state() != QTcpSocket::ConnectedState)
        return;

    socket.close();
}


} // namespace network
