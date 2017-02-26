#ifndef CLIENTINFO_HPP
#define CLIENTINFO_HPP

#include <QObject>
#include <QHostAddress>

namespace sync {

class ClientInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QHostAddress address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(quint16 bufferProgress READ bufferProgress WRITE setBufferProgress NOTIFY bufferProgressChanged)
    Q_PROPERTY(quint16 playProgress READ playProgress WRITE setPlayProgress NOTIFY playProgressChanged)
    Q_PROPERTY(QString bufferString READ bufferString WRITE setBufferString NOTIFY bufferStringChanged)


private:
        QHostAddress m_address;
        quint16 m_port;
        QString m_name;
        quint16 m_bufferProgress;
        quint16 m_playProgress;
        QString m_bufferString;

public:
        ClientInfo(QObject *parent = 0);

        QHostAddress address();
        quint16 port();
        QString name();
        quint16 bufferProgress();
        quint16 playProgress();
        QString bufferString();

        void setAddress(QHostAddress address);
        void setPort(quint16 port);
        void setName(QString name);
        void setBufferProgress(quint16 bufferProgress);
        void setPlayProgress(quint16 playProgress);
        void setBufferString(QString bufferString);

signals:
        void addressChanged();
        void portChanged();
        void nameChanged();
        void bufferProgressChanged();
        void playProgressChanged();
        void bufferStringChanged();

public slots:
};

} // namespace sync

#endif // CLIENTINFO_HPP
