#include "protocol.hpp"

#define PROTOCOL_QT_VERSION QDataStream::Qt_5_2

// Qt
#include <qdatastream.h>
#include <qiodevice.h>

namespace sync {

Protocol::Protocol() {

}

QDataStream & operator<< (QDataStream& stream, const Protocol::Packet& packet) {
    stream << (int)packet.phase;
    stream << packet.data;
    return stream;
}

QDataStream & operator>> (QDataStream& stream, Protocol::Packet& packet) {
    int iPhase = 0;
    stream >> iPhase >> packet.data;
    packet.phase = static_cast<Protocol::connectionPhase>(iPhase);
    return stream;
}

QDataStream & operator<< (QDataStream& stream, const mplayer::state& playerState) {
    stream << (int)playerState.playState;
    stream << playerState.playTime;
    return stream;
}

QDataStream & operator>> (QDataStream& stream, mplayer::state& playerState) {
    int iPlayState = 0;
    stream >> iPlayState >> playerState.playTime;

    playerState.playState = static_cast<mplayer::playState>(iPlayState);

    return stream;
}

QDataStream & operator<< (QDataStream& stream, const Protocol::SyncData& data) {
    stream << data.playerState;
    return stream;
}

QDataStream & operator>> (QDataStream& stream, Protocol::SyncData& packet) {
    int iPlayState = 0;
    stream >> iPlayState >> packet.playerState.playTime;

    packet.playerState.playState = static_cast<mplayer::playState>(iPlayState);
    return stream;
}

QDataStream & operator<< (QDataStream& stream, const Protocol::InitData& data) {
    stream << data.name;
    return stream;
}

QDataStream & operator>> (QDataStream& stream, Protocol::InitData& data) {
    stream >> data.name;
    return stream;
}

void Protocol::toInitPacket(QByteArray &buffer, QString name) {
    // Turn data into byte array
    InitData data = { name };
    QByteArray dataBytes;
    QDataStream outData(&dataBytes, QIODevice::Truncate);
    outData.setVersion(PROTOCOL_QT_VERSION);
    outData << data;

    // Pass packet into buffer
    QDataStream out(&buffer, QIODevice::Truncate);
    out.setVersion(PROTOCOL_QT_VERSION);

    Packet packet = {INIT, dataBytes};
    out << packet;
}

void Protocol::toSyncPacket(QByteArray &buffer, mplayer::state playerState) {
    // Turn data into byte array
    SyncData data = { playerState };
    QByteArray dataBytes;
    QDataStream outData(&dataBytes, QIODevice::Truncate);
    outData.setVersion(PROTOCOL_QT_VERSION);
    outData << data;

    // Pass packet into buffer
    QDataStream out(&buffer, QIODevice::Truncate);
    out.setVersion(PROTOCOL_QT_VERSION);

    Packet packet = {SYNC, dataBytes};
    out << packet;
}

QString Protocol::toName(QByteArray &buffer) {
    QDataStream in(&buffer, QIODevice::ReadOnly);
    in.setVersion(PROTOCOL_QT_VERSION);

    InitData data;
    in >> data;

    return data.name;
}

mplayer::state Protocol::toPlayerState(QByteArray &buffer) {
    QDataStream in(&buffer, QIODevice::ReadOnly);
    in.setVersion(PROTOCOL_QT_VERSION);

    SyncData data;
    in >> data;

    return data.playerState;
}

Protocol::Packet Protocol::toPacket(QByteArray &buffer) {
    QDataStream in(&buffer, QIODevice::ReadOnly);
    in.setVersion(PROTOCOL_QT_VERSION);

    Packet packet;
    in >> packet;

    return packet;
}

} // namespace network

