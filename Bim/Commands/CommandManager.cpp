#include "CommandManager.h"
void CommandManager::SetCommand(std::shared_ptr<ICommand> cmd) {
	if (m_CurrentCommand)
		m_CurrentCommand->Cancel();
	m_CurrentCommand = cmd;
}
void CommandManager::ClearCommand() {
	if (m_CurrentCommand)
		m_CurrentCommand->Cancel();
	m_CurrentCommand = nullptr;
}
void CommandManager::HandleMousePress(QMouseEvent* e) {
	if (m_CurrentCommand)
		m_CurrentCommand->OnMousePress(e);
}
void CommandManager::HandleMouseMove(QMouseEvent* e) {
	if (m_CurrentCommand)
		m_CurrentCommand->OnMouseMove(e);
}
void CommandManager::HandleMouseRelease(QMouseEvent* e) {
	if (m_CurrentCommand)
		m_CurrentCommand->OnMouseRelease(e);
}
void CommandManager::HandleDraw() {
	if (m_CurrentCommand)
		m_CurrentCommand->Draw();
}