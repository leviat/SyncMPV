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
    static void toInitPacket(QByteArray &buffer, QString name);
    static QString toName(QByteArray &buffer);
    static void toSyncPacket(QByteArray &buffer, mplayer::state playerState);
    static mplayer::state toPlayerState(QByteArray &syncPacket);

private:
    Protocol();
};

} // namespace network

#endif // PROTOCOL_HPP
