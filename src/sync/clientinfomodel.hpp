#ifndef CLIENTINFOMODEL_HPP
#define CLIENTINFOMODEL_HPP

#include <QAbstractTableModel>
#include <sync/clientinfo.hpp>

namespace sync {

class ClientInfoModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Roles {
        Address = Qt::UserRole + 1,
        Port,
        Name,
        BufferProgress,
        PlayProgress,
        BufferString
    };

private:
    QList<ClientInfo*> m_clients; //TODO: perhaps change to shared_ptr

public:
    ClientInfoModel(QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
    void addClientInfo(ClientInfo* clientInfo);
    void setName(QHostAddress address, QString name);
    void setBufferProgress(QHostAddress address, quint16 bufferProgress);
    void setPlayProgress(QHostAddress address, quint16 playProgress);
    void setBufferString(QHostAddress address, QString bufferString);
    void removeClientInfo(QHostAddress address);


};

} // namespace sync

#endif // CLIENTINFOMODEL_HPP
