#ifndef COMMAND_SYSTEM_H
#define COMMAND_SYSTEM_H

#include <QObject>
#include <vector>
#include <memory>

using namespace std;

class Command
{
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class CommandSystem : public QObject
{
    Q_OBJECT

public:
    explicit CommandSystem(QObject *parent = nullptr);
    
    // 以 unique_ptr 接收命令所有权
    void execute_command(unique_ptr<Command> command);
    void undo();
    void redo();
    
    bool can_undo() const;
    bool can_redo() const;

private:
    vector<unique_ptr<Command>> undo_stack_;
    vector<unique_ptr<Command>> redo_stack_;
    
    // 【扩展点】添加命令历史限制、事务、合并命令等
};

#endif // COMMAND_SYSTEM_H
