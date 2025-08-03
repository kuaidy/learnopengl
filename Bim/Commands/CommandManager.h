#pragma once
#include <stack>
#include <memory>
#include "ICommand.h"

class CommandManager
{
public:
    void SetCommand(std::shared_ptr<ICommand> cmd);
    void ClearCommand();
    void HandleFinish(std::shared_ptr<Bim::Document> document);
    void HandleMousePress(QMouseEvent* e);
    void HandleMouseMove(QMouseEvent* e);
    void HandleMouseRelease(QMouseEvent* e);
    void HandleDraw();
private:
    std::shared_ptr<ICommand> current_command = nullptr;
};

