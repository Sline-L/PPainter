#ifndef SYNCMANAGER_H
#define SYNCMANAGER_H

#include <QObject>
#include <QQueue>
#include <QTimer>

#include "network_manager.h"
#include "protocol_handler.h"
class SyncManager : public QObject
{
    Q_OBJECT
public:
    // 同步本地操作
    void sync_local_operation(const Operation& op);
    // 处理远程操作
    void handle_remote_operation(const QJsonObject& op_data);
    // 解决操作冲突
    void resolve_conflicts();
    
private:
    NetworkManager* network_manager_;
    QQueue<Operation> pending_operations_;
    quint64 local_timestamp_;
};

#endif
