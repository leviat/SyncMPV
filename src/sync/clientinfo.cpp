#include "clientinfo.hpp"

namespace sync {

ClientInfo::ClientInfo(QHostAddress address, quint16 port, QString name, quint16 bufferProgress, quint16 playProgress, QObject *parent)
    : address(address), port(port), name(name), bufferProgress(bufferProgress), playProgress(playProgress), QObject(parent)
{

}

} // namespace sync
