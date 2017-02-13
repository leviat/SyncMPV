#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QThread>
#include <QTcpSocket>

namespace network {



/*====================================================================*/
/* CLASS DEFINITIONS                                                                                      */
/*====================================================================*/

class Client : public QObject
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
    Client();

public slots:
    void connect();
    void disconnect();

private slots:
    void sendName();

};

} // namespace network

#endif // CLIENT_HPP
