#include "LineCommand.h"


LineCommand::LineCommand(std::shared_ptr<CommandManager> commandManager,
	std::shared_ptr<Bim::Document> document):
	m_CommandManager(commandManager),
	m_Document(document)
{
	m_Line = std::make_shared<Line>(m_CommandManager,m_Document);
	m_Line->elementType = ElementType::Line;
	m_Line->opengl_funcs = RenderContext::Instance().gl;
	m_Line->matrix_model = RenderContext::Instance().matrix_model;
	m_Line->matrix_view = RenderContext::Instance().matrix_view;
	m_Line->matrix_projection = RenderContext::Instance().matrix_projection;
}

void LineCommand::Execute() {

}
void LineCommand::Finish(std::shared_ptr<Bim::Document> document) {
	document->AddElement(m_Line);
}
void LineCommand::Cancel() {

}
void LineCommand::OnMousePress(QMouseEvent* event) {
	QVector3D p = Transform::ScreenToWorld(
		event->pos().x(),
		event->pos().y(),
		RenderContext::Instance().width,
		RenderContext::Instance().height,
		m_Line->matrix_projection,
		m_Line->matrix_view);
	Bim::Vertex vertex;
	vertex.Position = p;
	m_Line->mesh.vertices.push_back(vertex);
}
void LineCommand::OnMouseMove(QMouseEvent* event) {
	m_Line->last_vertex.Position = Transform::ScreenToWorld(
		event->pos().x(),
		event->pos().y(),
		RenderContext::Instance().width,
		RenderContext::Instance().height,
		m_Line->matrix_projection,
		m_Line->matrix_view);
}
void LineCommand::OnMouseRelease(QMouseEvent* event) {

}
void LineCommand::OnDraw() {
	m_Line->Draw();
}