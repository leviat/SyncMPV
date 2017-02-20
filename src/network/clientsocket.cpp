#include "clientsocket.hpp"

#include <sync/protocol.hpp>

// Qt
#include <QTcpSocket>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QNetworkInterface>

namespace network {

ClientSocket::ClientSocket()
{
    QObject::connect(&socket, static_cast<void(QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error),
                     [=](QTcpSocket::SocketError socketError){ qDebug() << "Client error: " << socketError; });

    QObject::connect(&socket, &QTcpSocket::stateChanged,  [=](){ qDebug() << "Client state: " << socket.state(); });
    QObject::connect(&socket, &QTcpSocket::connected, this, &ClientSocket::connected);
    QObject::connect(&socket, &QTcpSocket::connected, this, &ClientSocket::newData);

}

void ClientSocket::connect(QHostAddress& address) {
    if (socket.state() != QTcpSocket::UnconnectedState)
        return;

    socket.bind(8001);
    socket.connectToHost(address, 8000);
    socket.setSocketOption(QTcpSocket::LowDelayOption, 1);

}

void ClientSocket::send(QByteArray& packet) {
    if (socket.state() != QTcpSocket::ConnectedState) {
        qDebug() << "Client tries to send data but is not connected.";
        return;
    }

    socket.write(packet);
}

void ClientSocket::disconnect() {
    if (socket.state() != QTcpSocket::ConnectedState)
        return;

    socket.close();
}


} // namespace network
