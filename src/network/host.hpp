#ifndef HOST_HPP
#define HOST_HPP

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

    /*====================================================================*/
    /* FUNCTIONS                                                                                                   */
    /*====================================================================*/

public:
    Host();

private:
    void run();

signals:
    void serverListening();
    void serverError();

public slots:
    void openConnection();
    void closeConnection();

private slots:
    void addClient();
    void checkClientName();

};

} // namespace network

#endif // HOST_HPP
