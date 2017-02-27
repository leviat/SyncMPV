#ifndef HOST_HPP
#define HOST_HPP

#include <network/hostsocket.hpp>
#include <sync/clientinfo.hpp>
#include <sync/clientinfomodel.hpp>

#include <memory>

#include <mplayer/mpvobject.hpp>

// Qt
#include <QObject>

namespace sync {

class Host : public QObject
{
    Q_OBJECT

private:
    network::HostSocket m_socket;
    ClientInfoModel* m_clientInfoModel;
    mplayer::MpvObject* m_mpv;
    mplayer::mediumInfo m_mediumInfo;
    QSet<QHostAddress> m_waitingForBuffer;

public:
    Host(QObject *parent = 0);
    void setClientInfoModel(ClientInfoModel* clientInfoModel);
    void setMpv(mplayer::MpvObject* mpv_instance);

signals:
    void waitForBuffer();
    void bufferIsHealthy();

public slots:
    void broadcastPlayerState(mplayer::state state);
    void processPackage();
    void addClient(QTcpSocket* client);
    void openConnection();
    void closeConnection();
    void removeClient(QHostAddress address, quint16 port);
    void setMediumInfo(mplayer::mediumInfo mediumInfo);

};

} // namespace sync
#endif // HOST_HPP
