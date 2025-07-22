#pragma once
#include <stack>
#include <memory>
#include "ICommand.h"
class CommandManager
{
    std::stack<std::shared_ptr<ICommand>> undoStack;
    std::stack<std::shared_ptr<ICommand>> redoStack;
public:
    void Do(std::shared_ptr<ICommand> cmd);
    void Undo();
    void Redo();
    enum CommandMode commandMode;
    enum CommandState commandState;
};

