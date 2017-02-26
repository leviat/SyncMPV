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

    Q_PROPERTY(QString hostAddress MEMBER m_hostAddress)

private:
    network::ClientSocket m_socket;
    mplayer::MpvObject* m_mpv;
    mplayer::state m_currentState;
    QString m_hostAddress;

public:
    explicit Client(QObject *parent = 0);
    void sendName();
    void processPackage();

signals:
    void propertyChange(QString name, QVariant value);
    void command(QVariant params);

public slots:
    void connect();
    void disconnect();
    void setState(mplayer::state state);
    void sendPlayerState(mplayer::state state);
    void setMpv(mplayer::MpvObject* mpvInstance);

};

} // namespace sync

#endif // CLIENT_HPP
