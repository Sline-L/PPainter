#ifndef SYNC_MANAGER_H
#define SYNC_MANAGER_H

#include <QObject>

class NetworkManager;
class ProtocolHandler;
class PixelCanvas;

class SyncManager : public QObject
{
    Q_OBJECT

public:
    explicit SyncManager(QObject *parent = nullptr);
    
    void set_canvas(PixelCanvas *canvas);
    void set_network_manager(NetworkManager *network_manager);

private slots:
    void on_canvas_changed();
    void on_remote_drawing_data(int x, int y, const QColor &color);

private:
    PixelCanvas *canvas_;
    NetworkManager *network_manager_;
    ProtocolHandler *protocol_handler_;
    
    // 【扩展点】添加冲突解决机制
    // 【扩展点】添加同步状态管理
};

#endif // SYNC_MANAGER_H
