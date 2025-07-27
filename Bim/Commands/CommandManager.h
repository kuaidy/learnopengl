#pragma once
#include <stack>
#include <memory>
#include "ICommand.h"
class CommandManager
{
public:
    void SetCommand(std::shared_ptr<ICommand> cmd);
    void ClearCommand();
    void HandleMousePress(QMouseEvent* e);
    void HandleMouseMove(QMouseEvent* e);
    void HandleMouseRelease(QMouseEvent* e);
    void HandleDraw();
private:
    std::shared_ptr<ICommand> m_CurrentCommand = nullptr;
};

