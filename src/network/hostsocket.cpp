#include "hostsocket.hpp"

#include <sync/protocol.hpp>

// Qt
#include <QTcpSocket>
#include <QTcpServer>

namespace network {

/*!
    \class network::HostSocket
    \inmodule network
    \brief The HostSocket provides a server for clients to connect to.
*/

/*!
 * \brief Constructs a closed HostSocket.
 */
HostSocket::HostSocket()
{
    status = NONE;
    connect(&server, &QTcpServer::newConnection, this, &HostSocket::addClient);
}

/*!
 * \brief Destroys the socket.
 */
HostSocket::~HostSocket() {
    closeConnection();
}

/*!
 * \brief Opens the connection on the \a port.
 */
void HostSocket::openConnection(quint16 port) {
    if (status != CONNECTION_OPEN) {
        if (!server.listen(QHostAddress::Any, port)) {
            emit serverError(); // TODO: dont forget to connect this signal
            qDebug() << "Could not open server";
            return;
        }

        status = CONNECTION_OPEN;
    }
}

/*!
 * \brief Accepts the next pending client and adds it to the broadcast list.
 */
void HostSocket::addClient() {
    QTcpSocket *clientConnection = server.nextPendingConnection();
    clientConnection->setSocketOption(QTcpSocket::LowDelayOption, 1);
    clients.append(clientConnection);

    connect(clientConnection, &QTcpSocket::disconnected, this, [=]() { clients.removeOne(reinterpret_cast<QTcpSocket*>(sender())); });
    connect(clientConnection, &QTcpSocket::disconnected, clientConnection, &QObject::deleteLater);

    emit newClient(clientConnection);
    connect(clientConnection, &QTcpSocket::readyRead, this, [=]() { emit newData(clientConnection); });
}

/*!
 * \brief Broadcasts the \a packet to all clients.
 */
void HostSocket::broadcast(QByteArray& packet) {
    for (QTcpSocket* client : clients) {
        if (client->state() == QTcpSocket::ConnectedState)
            client->write(packet);
    }
}

/*!
 * \brief Closes the connection.
 */
void HostSocket::closeConnection() {
    if (status == CONNECTION_OPEN) {
        server.close();
        status = CONNECTION_CLOSED;
    }
}

// Misc. documentation

/*!
  \fn void HostSocket::serverError()
  \brief Is emitted when the server could not be properly opened.
*/

/*!
  \fn void HostSocket::newClient(QTcpSocket* client);
  \brief Is emitted when a new \a client connects to the HostSocket.
*/

}
