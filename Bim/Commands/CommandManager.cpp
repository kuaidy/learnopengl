#include "CommandManager.h"
void CommandManager::SetCommand(std::shared_ptr<ICommand> cmd) {
	if (current_command)
		current_command->Cancel();
	current_command = cmd;
}
void CommandManager::ClearCommand() {
	if (current_command)
		current_command->Cancel();
	current_command = nullptr;
}
void CommandManager::HandleMousePress(QMouseEvent* e) {
	if (current_command)
		current_command->OnMousePress(e);
}
void CommandManager::HandleMouseMove(QMouseEvent* e) {
	if (current_command)
		current_command->OnMouseMove(e);
}
void CommandManager::HandleMouseRelease(QMouseEvent* e) {
	if (current_command)
		current_command->OnMouseRelease(e);
}
void CommandManager::HandleDraw() {
	if (current_command)
		current_command->OnDraw();
}
void CommandManager::HandleFinish(std::shared_ptr<Bim::Document> document) {
	if (current_command)
		current_command->Finish(document);
	current_command = nullptr;
}