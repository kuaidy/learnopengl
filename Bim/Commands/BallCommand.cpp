#include "BallCommand.h"
BallCommand::BallCommand(std::shared_ptr<CommandManager> commandmanager,
	std::shared_ptr<Bim::Document> document)
	:m_command_manager(commandmanager),
	m_document(document)
{
	m_ball = std::make_shared<Ball>(m_command_manager,m_document);
	m_ball->elementType = ElementType::Volume;
	m_ball->matrix_model = RenderContext::Instance().matrix_model;
	m_ball->matrix_view = RenderContext::Instance().matrix_view;
	m_ball->matrix_projection = RenderContext::Instance().matrix_projection;
}
void BallCommand::Execute() {

}
void BallCommand::Finish(std::shared_ptr<Bim::Document> document) {
	document->AddElement(m_ball);
}
void BallCommand::Cancel() {

}
void BallCommand::OnMousePress(QMouseEvent* event) {

}
void BallCommand::OnMouseMove(QMouseEvent* event) {

}
void BallCommand::OnMouseRelease(QMouseEvent* event) {

}
void BallCommand::OnDraw() {
	if (m_ball) {
		m_ball->Draw();
	}
}