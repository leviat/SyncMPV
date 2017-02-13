#include "client.hpp"

#include <network/protocol.hpp>

// Qt
#include <QTcpSocket>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QNetworkInterface>

namespace network {

Client::Client()
{
    QObject::connect(&socket, static_cast<void(QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error),
                     [=](QTcpSocket::SocketError socketError){ qDebug() << "Client error: " << socketError; });

    QObject::connect(&socket, &QTcpSocket::stateChanged,  [=](){ qDebug() << "Client state: " << socket.state(); });
    QObject::connect(&socket, &QTcpSocket::connected, this, &Client::sendName);

}

void Client::connect() {
    if (socket.state() != QTcpSocket::UnconnectedState)
        return;

    socket.bind(8001);
    socket.connectToHost("127.0.0.1", 8000, QIODevice::ReadWrite, QTcpSocket::IPv4Protocol);

}

void Client::sendName() {
    if (socket.state() != QTcpSocket::ConnectedState)
        return;

    QByteArray buffer = QByteArray();
    Protocol::initiationPacket(buffer, "client_moop");
    socket.write(buffer);
}

void Client::disconnect() {
    if (socket.state() != QTcpSocket::ConnectedState)
        return;

    socket.close();
}

} // namespace network
