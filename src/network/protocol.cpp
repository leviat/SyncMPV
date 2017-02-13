#include "protocol.hpp"

// Qt
#include <qdatastream.h>
#include <qiodevice.h>

namespace network {

enum connectionPhase {
    INIT,
    SYNC
};

struct initPacket {
    network::connectionPhase phase;
    QString name;
};

Protocol::Protocol() {

}

QDataStream & operator<< (QDataStream& stream, const initPacket& packet) {
    stream << (int)packet.phase;
    stream << packet.name;
    return stream;
}

QDataStream & operator>> (QDataStream& stream, initPacket& packet) {
    int iPhase = 0;
    stream >> iPhase >> packet.name;
    packet.phase = static_cast<connectionPhase>(iPhase);
    return stream;
}

void Protocol::initiationPacket(QByteArray &buffer, QString name) {
    QDataStream out(&buffer, QIODevice::Truncate);
    out.setVersion(QDataStream::Qt_5_2);

    initPacket packet = {INIT, name};
    out << packet;
}

QString Protocol::toName(QByteArray &initiationPacket) {
    QDataStream in(&initiationPacket, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_2);

    initPacket packet = {INIT, ""};
    in >> packet;
    return packet.name;
}


} // namespace network

