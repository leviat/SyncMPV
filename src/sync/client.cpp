#include "client.hpp"

#include <sync/protocol.hpp>

#include <QHostAddress>

namespace sync {

Client::Client(QObject *parent) : QObject(parent)
{
}

void Client::connect(){
    QObject::connect(&socket, &network::ClientSocket::connected, this, &Client::sendName);

    QHostAddress host = QHostAddress("127.0.0.1");
    socket.connect(host);
}

void Client::disconnect() {
    socket.disconnect();
}

void Client::sendName() {
    QObject::disconnect(&socket, &network::ClientSocket::connected, this, &Client::sendName);

    QByteArray buffer = QByteArray();
    sync::Protocol::toInitPacket(buffer, "client_moop");
    socket.send(buffer);
}

void Client::processSyncPacket() {
    QObject* obj = sender();
    QByteArray packet = reinterpret_cast<QTcpSocket*>(obj)->readAll();
    qDebug() << "Host play time: " << sync::Protocol::toPlayerState(packet).playTime;
}

} // namespace sync
