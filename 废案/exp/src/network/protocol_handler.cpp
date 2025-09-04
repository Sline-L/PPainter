#include "protocol_handler.h"

#include <QDataStream>
#include <QIODevice>   // 必须包含，以便使用 QIODevice::WriteOnly / ReadOnly

using namespace std;

ProtocolHandler::ProtocolHandler(QObject *parent)
    : QObject(parent)
{
}

QByteArray ProtocolHandler::encode_drawing_data(int x, int y, const QColor &color)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly); // 现在 QIODevice 可用
    
    stream << x << y << color.rgb();
    
    // 【扩展点】添加消息头、时间戳等
    return data;
}

void ProtocolHandler::decode_drawing_data(const QByteArray &data)
{
    QDataStream stream(data);
    
    int x = 0;
    int y = 0;
    QRgb color_rgb = 0;
    
    stream >> x >> y >> color_rgb;
    
    QColor color = QColor::fromRgb(color_rgb);
    emit drawing_data_received(x, y, color);
    
    // 【扩展点】添加数据验证
}
