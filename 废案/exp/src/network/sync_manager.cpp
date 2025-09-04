#include "sync_manager.h"
#include "network_manager.h"
#include "protocol_handler.h"
#include "../core/pixel_canvas.h"

using namespace std;

SyncManager::SyncManager(QObject *parent)
    : QObject(parent)
    , canvas_(nullptr)
    , network_manager_(nullptr)
    , protocol_handler_(new ProtocolHandler(this))
{
    connect(protocol_handler_, &ProtocolHandler::drawing_data_received,
            this, &SyncManager::on_remote_drawing_data);
}

void SyncManager::set_canvas(PixelCanvas *canvas)
{
    canvas_ = canvas;
    if (canvas_) {
        connect(canvas_, &PixelCanvas::canvas_changed,
                this, &SyncManager::on_canvas_changed);
    }
}

void SyncManager::set_network_manager(NetworkManager *network_manager)
{
    network_manager_ = network_manager;
    // 【扩展点】连接网络信号
}

void SyncManager::on_canvas_changed()
{
    // 【扩展点】发送画布变更到网络
}

void SyncManager::on_remote_drawing_data(int x, int y, const QColor &color)
{
    if (canvas_) {
        canvas_->set_pixel(x, y, color);
    }
}
