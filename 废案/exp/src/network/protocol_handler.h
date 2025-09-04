#ifndef PROTOCOL_HANDLER_H
#define PROTOCOL_HANDLER_H

#include <QObject>
#include <QByteArray>
#include <QColor>      // 需要 QColor 的声明

class ProtocolHandler : public QObject
{
    Q_OBJECT

public:
    explicit ProtocolHandler(QObject *parent = nullptr);
    
    QByteArray encode_drawing_data(int x, int y, const QColor &color);
    void decode_drawing_data(const QByteArray &data);

signals:
    void drawing_data_received(int x, int y, const QColor &color);

private:
    // 【扩展点】定义协议格式
    // 【扩展点】添加数据压缩
};

#endif // PROTOCOL_HANDLER_H
