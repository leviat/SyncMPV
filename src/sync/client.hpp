#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <network/clientsocket.hpp>
#include <mplayer/mpvobject.hpp>

// Qt
#include <QObject>

namespace sync {

class Client : public QObject
{
    Q_OBJECT

private:
    network::ClientSocket m_socket;
    mplayer::MpvObject* m_mpv;
    mplayer::state m_currentState;

public:
    explicit Client(QObject *parent = 0);
    void sendName();
    void processPackage();
    void setMpv(mplayer::MpvObject* mpvInstance);

signals:
    void propertyChange(QString name, QVariant value);

public slots:
    void connect();
    void disconnect();
    void setState(mplayer::state state);

};

} // namespace sync

#endif // CLIENT_HPP
