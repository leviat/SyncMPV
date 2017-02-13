#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <qbytearray.h>
#include <qstring.h>

namespace network {

class Protocol
{
public:
    static void initiationPacket(QByteArray &buffer, QString name);
    static QString toName(QByteArray &initiationPacket);

private:
    Protocol();
};

} // namespace network

#endif // PROTOCOL_HPP
