#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    
    bool start_server(quint16 port);
    bool connect_to_server(const QString &host, quint16 port);
    void disconnect_from_server();

signals:
    void connected();
    void disconnected();
    void data_received(const QByteArray &data);

private slots:
    void on_new_connection();
    void on_data_ready();
    void on_disconnected();

private:
    QTcpServer *server_;
    QTcpSocket *socket_;
    
    // 【扩展点】添加多客户端支持
    // 【扩展点】添加连接管理
};

#endif // NETWORK_MANAGER_H
