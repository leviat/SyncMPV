#ifndef HOST_HPP
#define HOST_HPP

#include <mplayer/mpvobject.hpp>

// Qt
#include <QTcpServer>

namespace network {

/*====================================================================*/
/* CLASS DEFINITIONS                                                                                      */
/*====================================================================*/

class Host : public QObject
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
    Host::state status;
    QVector<QTcpSocket*> clients;

    /*====================================================================*/
    /* FUNCTIONS                                                                                                   */
    /*====================================================================*/

public:
    Host();
    ~Host();

private:
    void run();

signals:
    void serverListening();
    void serverError();

public slots:
    void openConnection();
    void closeConnection();
    void broadcastPlayerState();

private slots:
    void addClient();
    void checkClientName();

};

} // namespace network

#endif // HOST_HPP
