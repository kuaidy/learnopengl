#include "CommandManager.h"
void Do(std::shared_ptr<ICommand> cmd) {
	cmd->Execute();
}
void Undo() {

}
void Redo() {

}