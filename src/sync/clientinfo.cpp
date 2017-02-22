#include "clientinfo.hpp"

namespace sync {

ClientInfo::ClientInfo(QObject *parent) : QObject(parent) {

}

QHostAddress ClientInfo::address() {
    return m_address;
}

quint16 ClientInfo::port() {
    return m_port;
}

QString ClientInfo::name() {
    return m_name;
}

quint16 ClientInfo::bufferProgress() {
    return m_bufferProgress;
}

quint16 ClientInfo::playProgress() {
    return m_playProgress;
}

void ClientInfo::setAddress(QHostAddress address) {
    m_address = address;
    emit addressChanged();
}

void ClientInfo::setBufferProgress(quint16 bufferProgress) {
    m_bufferProgress = bufferProgress;
    emit bufferProgressChanged();
}

void ClientInfo::setName(QString name) {
    m_name = name;
    emit nameChanged();
}

void ClientInfo::setPlayProgress(quint16 playProgress) {
    m_playProgress = playProgress;
    emit playProgressChanged();
}

void ClientInfo::setPort(quint16 port) {
    m_port = port;
    emit portChanged();
}


} // namespace sync
