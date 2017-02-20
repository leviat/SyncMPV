#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP

#include <QThread>
#include <QTcpSocket>

namespace network {

class ClientSocket : public QObject
{
    Q_OBJECT

    /*====================================================================*/
    /* VARIABLES                                                                                                    */
    /*====================================================================*/

private:
    QTcpSocket socket;

    /*====================================================================*/
    /* FUNCTIONS                                                                                                   */
    /*====================================================================*/

public:
    ClientSocket();
    ~ClientSocket();
    QByteArray readAll();

signals:
    void connected();
    void newData();

public slots:
    void connect(QHostAddress& address);
    void disconnect();
    void send(QByteArray &packet);

};

} // namespace network

#endif // CLIENTSOCKET_HPP
