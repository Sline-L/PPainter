#ifndef COMMANDSYSTEM_H
#define COMMANDSYSTEM_H

#include <QObject>
class CommandSystem : public QObject
{
    Q_OBJECT
public:
    // 执行命令
    void execute_command(Command* command);
    // 撤销操作
    void undo();
    // 重做操作
    void redo();
    // 清空历史
    void clear_history();
    
    bool can_undo() const;
    bool can_redo() const;
    
private:
    QStack<Command*> undo_stack_;
    QStack<Command*> redo_stack_;
    int max_history_size_;
};

#endif