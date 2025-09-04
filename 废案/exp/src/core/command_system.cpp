#include "command_system.h"

using namespace std;

CommandSystem::CommandSystem(QObject *parent)
    : QObject(parent)
{
}

void CommandSystem::execute_command(unique_ptr<Command> command)
{
    if (!command) return;
    command->execute();
    // 将命令所有权移动到 undo 栈
    undo_stack_.push_back(std::move(command));
    // 清空重做栈
    redo_stack_.clear();
}

void CommandSystem::undo()
{
    if (undo_stack_.empty()) return;
    // 将最后一个命令移出 undo 栈，执行 undo，然后移动到 redo 栈
    unique_ptr<Command> cmd = std::move(undo_stack_.back());
    undo_stack_.pop_back();
    if (cmd) {
        cmd->undo();
        redo_stack_.push_back(std::move(cmd));
    }
}

void CommandSystem::redo()
{
    if (redo_stack_.empty()) return;
    // 将最后一个命令移出 redo 栈，重新执行，然后移动回 undo 栈
    unique_ptr<Command> cmd = std::move(redo_stack_.back());
    redo_stack_.pop_back();
    if (cmd) {
        cmd->execute();
        undo_stack_.push_back(std::move(cmd));
    }
}

bool CommandSystem::can_undo() const
{
    return !undo_stack_.empty();
}

bool CommandSystem::can_redo() const
{
    return !redo_stack_.empty();
}
