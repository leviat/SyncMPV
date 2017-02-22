#include "clientinfomodel.hpp"

// Qt
#include <QVariant>
#include <QObject>

namespace sync {

ClientInfoModel::ClientInfoModel(QObject *parent) : QAbstractTableModel(parent)
{
    // Some test code

    ClientInfo* cInfo = new ClientInfo();
    cInfo->setAddress(QHostAddress("127.0.0.1"));
    cInfo->setName("Leo");
    cInfo->setPort(8000);
    cInfo->setBufferProgress(50);
    cInfo->setPlayProgress(20);
    addClientInfo(cInfo);
}

QHash<int, QByteArray> ClientInfoModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Address] = "address";
    roles[Port] = "port";
    roles[Name] = "name";
    roles[BufferProgress] = "bufferProgress";
    roles[PlayProgress] = "playProgress";

    return roles;
}

int ClientInfoModel::rowCount(const QModelIndex &parent) const {
    if (parent == QModelIndex())
        return m_clients.size();

    return 0;
}

int ClientInfoModel::columnCount(const QModelIndex &parent) const {
    if (parent == QModelIndex())
        return 5;

    return 0;
}

QVariant ClientInfoModel::data(const QModelIndex &index, int role) const {
    if (index.row() > m_clients.size() || index.row() < 0)
        return QVariant();

    auto clientInfo = m_clients.at(index.row());

    switch (role) {
        case Address:
            return  clientInfo->address().toString();
            break;
        case Port:
            return clientInfo->port();
            break;
        case Name:
            return clientInfo->name();
            break;
        case BufferProgress:
            return clientInfo->bufferProgress();
            break;
        case PlayProgress:
            return clientInfo->playProgress();
            break;
        default:
            return clientInfo->name();
            break;
    }

}

void ClientInfoModel::addClientInfo(ClientInfo* clientInfo)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_clients.append(clientInfo);
    endInsertRows();
}

} // namespace sync
