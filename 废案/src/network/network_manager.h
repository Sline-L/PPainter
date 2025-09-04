#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    // 连接到服务器
    void connect_to_server(const QString& host, int port);
    // 断开连接
    void disconnect_from_server();
    // 发送操作数据
    void send_operation(const QJsonObject& operation);
    
signals:
    void connected();
    void disconnected();
    void operation_received(const QJsonObject& operation);
    void error_occurred(const QString& error);
    
private slots:
    void on_socket_connected();
    void on_socket_disconnected();
    void on_data_received();
    
private:
    QTcpSocket* tcp_socket_;
    QJsonDocument parse_message(const QByteArray& data);
};

#endif