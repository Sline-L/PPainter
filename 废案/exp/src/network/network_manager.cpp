#include "network_manager.h"

using namespace std;

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , server_(new QTcpServer(this))
    , socket_(nullptr)
{
    connect(server_, &QTcpServer::newConnection, this, &NetworkManager::on_new_connection);
}

bool NetworkManager::start_server(quint16 port)
{
    return server_->listen(QHostAddress::Any, port);
}

bool NetworkManager::connect_to_server(const QString &host, quint16 port)
{
    socket_ = new QTcpSocket(this);
    connect(socket_, &QTcpSocket::connected, this, &NetworkManager::connected);
    connect(socket_, &QTcpSocket::disconnected, this, &NetworkManager::on_disconnected);
    connect(socket_, &QTcpSocket::readyRead, this, &NetworkManager::on_data_ready);
    
    socket_->connectToHost(host, port);
    return true;
}

void NetworkManager::disconnect_from_server()
{
    if (socket_) {
        socket_->disconnectFromHost();
    }
}

void NetworkManager::on_new_connection()
{
    // 【扩展点】处理新连接
}

void NetworkManager::on_data_ready()
{
    // 【扩展点】处理接收到的数据
}

void NetworkManager::on_disconnected()
{
    emit disconnected();
}
