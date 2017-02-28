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
    void connect(QString address, quint16 hostPort, quint16 clientPort);
    void disconnect();
    void send(QByteArray &packet);

};

} // namespace network

#endif // CLIENTSOCKET_HPP
