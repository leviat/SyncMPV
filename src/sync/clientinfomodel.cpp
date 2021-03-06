#include "clientinfomodel.hpp"

// Qt
#include <QVariant>
#include <QObject>

namespace sync {

/*!
    \class sync::ClientInfoModel
    \inmodule sync
    \brief The ClientInfoModel holds a table of ClientInfos to display in the host's sidebar.
*/

ClientInfoModel::ClientInfoModel(QObject *parent) : QAbstractTableModel(parent)
{
    /* -- Debug dummies
    ClientInfo* cInfo = new ClientInfo();
    cInfo->setAddress(QHostAddress("127.0.0.1"));
    cInfo->setName("Leo");
    cInfo->setPort(8000);
    cInfo->setBufferProgress(20);
    cInfo->setPlayProgress(20);
    cInfo->setBufferString("buffer: 5s+10kB");
    addClientInfo(cInfo);

    cInfo = new ClientInfo();
    cInfo->setAddress(QHostAddress("127.0.0.1"));
    cInfo->setName("Dominik");
    cInfo->setPort(8000);
    cInfo->setBufferProgress(10);
    cInfo->setPlayProgress(20);
    cInfo->setBufferString("buffer: 2s+100kB");
    addClientInfo(cInfo);
    -- */
}

QHash<int, QByteArray> ClientInfoModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Address] = "address";
    roles[Port] = "port";
    roles[Name] = "name";
    roles[BufferProgress] = "bufferProgress";
    roles[PlayProgress] = "playProgress";
    roles[BufferString] = "bufferString";

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
        case BufferString:
            return clientInfo->bufferString();
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

int ClientInfoModel::find(QHostAddress address, quint16 port) {
    for (int i = 0; i < m_clients.size(); i++) {
        if (m_clients[i]->address() == address && m_clients[i]->port() == port) {
            return i;
        }
    }

    return -1;
}

void ClientInfoModel::setName(QHostAddress address, quint16 port, QString name) {
    int i = find(address, port);

    if (i != -1) {
        m_clients[i]->setName(name);
        emit dataChanged(index(i, 2), index(i, 2));
    }
}

void ClientInfoModel::setBufferProgress(QHostAddress address, quint16 port, quint16 bufferProgress) {
    int i = find(address, port);

    if (i != -1) {
        m_clients[i]->setBufferProgress(bufferProgress);
        emit dataChanged(index(i, 3), index(i, 3));
    }
}

void ClientInfoModel::setPlayProgress(QHostAddress address, quint16 port, quint16 playProgress) {
    int i = find(address, port);

    if (i != -1) {
        m_clients[i]->setPlayProgress(playProgress);
        emit dataChanged(index(i, 4), index(i, 4));
    }
}

void ClientInfoModel::setBufferString(QHostAddress address, quint16 port, QString bufferString) {
    int i = find(address, port);

    if (i != -1) {
        m_clients[i]->setBufferString(bufferString);
        emit dataChanged(index(i, 5), index(i, 5));
    }
}


void ClientInfoModel::removeClientInfo(QHostAddress address, quint16 port) {
    int i = find(address, port);

    if (i != -1) {
        beginRemoveRows(QModelIndex(), i, i);
        m_clients.removeAt(i);
        endRemoveRows();
    }
}

} // namespace sync
