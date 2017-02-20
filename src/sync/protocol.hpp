#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <mplayer/mpvobject.hpp>

// Qt
#include <qbytearray.h>
#include <qstring.h>

namespace sync {

class Protocol
{

public:

    enum connectionPhase {
        INIT,
        SYNC
    };

    struct Packet {
        Protocol::connectionPhase phase;
        QByteArray data;
    };

    struct InitData {
        QString name;
    };

    struct SyncData {
        mplayer::state playerState;
    };


public:
    static void toInitPacket(QByteArray &buffer, QString name);
    static QString toName(QByteArray &buffer);
    static void toSyncPacket(QByteArray &buffer, mplayer::state playerState);
    static mplayer::state toPlayerState(QByteArray &syncPacket);
    static Protocol::Packet toPacket(QByteArray &buffer);


private:
    Protocol();
};

} // namespace network

#endif // PROTOCOL_HPP
