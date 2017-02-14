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

struct syncPacket {
    network::connectionPhase phase;
    mplayer::state playerState;
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

QDataStream & operator<< (QDataStream& stream, const syncPacket& packet) {
    stream << (int)packet.phase;
    stream << (int)packet.playerState.playState;
    stream << packet.playerState.playTime;
    return stream;
}

QDataStream & operator>> (QDataStream& stream, syncPacket& packet) {
    int iPhase = 0;
    int iPlayState = 0;
    stream >> iPhase >> iPlayState >> packet.playerState.playTime;

    packet.phase = static_cast<connectionPhase>(iPhase);
    packet.playerState.playState = static_cast<mplayer::playState>(iPlayState);

    return stream;
}

void Protocol::toInitPacket(QByteArray &buffer, QString name) {
    QDataStream out(&buffer, QIODevice::Truncate);
    out.setVersion(QDataStream::Qt_5_2);

    initPacket packet = {INIT, name};
    out << packet;
}

QString Protocol::toName(QByteArray &buffer) {
    QDataStream in(&buffer, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_2);

    initPacket packet = {};
    in >> packet;

    if (packet.phase != INIT) {
        qDebug() << "Invalid initiation packet detected.";
        throw 0; // TODO: disconnect that client then
    }

    return packet.name;
}

void Protocol::toSyncPacket(QByteArray &buffer, mplayer::state playerState) {
    QDataStream out(&buffer, QIODevice::Truncate);
    out.setVersion(QDataStream::Qt_5_2);

    syncPacket packet = {SYNC, playerState};
    out << packet;
}

mplayer::state Protocol::toPlayerState(QByteArray &buffer) {
    QDataStream in(&buffer, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_2);

    syncPacket packet = {};
    in >> packet;

    if (packet.phase != SYNC) {
        qDebug() << "Invalid sync packet detected.";
        throw 0; // TODO: disconnect that client then
    }

    return packet.playerState;
}


} // namespace network

