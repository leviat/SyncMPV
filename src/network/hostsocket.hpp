#ifndef HOSTSOCKET_HPP
#define HOSTSOCKET_HPP

#include <mplayer/mpvobject.hpp>

// Qt
#include <QTcpServer>

namespace network {

class HostSocket : public QObject
{
    Q_OBJECT

    enum state {
        NONE,
        CONNECTION_OPEN,
        CONNECTION_CLOSED
    };

    /*====================================================================*/
    /* VARIABLES                                                                                                    */
    /*====================================================================*/

private:
    QTcpServer server;
    HostSocket::state status;
    QList<QTcpSocket*> clients;

    /*====================================================================*/
    /* FUNCTIONS                                                                                                   */
    /*====================================================================*/

public:
    HostSocket();
    ~HostSocket();

private:
    void run();

signals:
    void serverError();
    void newClient(QTcpSocket* client);

public slots:
    void openConnection();
    void closeConnection();
    void broadcast(QByteArray& packet);

private slots:
    void addClient();

};

} // namespace network

#endif // HOSTSOCKET_HPP
