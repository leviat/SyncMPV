#ifndef CLIENTINFO_HPP
#define CLIENTINFO_HPP

#include <QObject>
#include <QHostAddress>

namespace sync {

class ClientInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QHostAddress address MEMBER address)
    Q_PROPERTY(quint16 port MEMBER port NOTIFY portChanged)
    Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)
    Q_PROPERTY(quint16 bufferProgress MEMBER bufferProgress NOTIFY bufferProgressChanged)
    Q_PROPERTY(quint16 playProgress MEMBER playProgress NOTIFY playProgressChanged)

public:
        QHostAddress address;
        quint16 port;
        QString name;
        quint16 bufferProgress;
        quint16 playProgress;

public:
        ClientInfo(QHostAddress address, quint16 port, QString name, quint16 bufferProgress, quint16 playProgress, QObject *parent = 0);

signals:
        void addressChanged();
        void portChanged();
        void nameChanged();
        void bufferProgressChanged();
        void playProgressChanged();

public slots:
};

} // namespace sync

#endif // CLIENTINFO_HPP
