#include "LineCommand.h"


LineCommand::LineCommand(std::shared_ptr<CommandManager> commandManager,
	std::shared_ptr<Bim::Document> document):
	m_CommandManager(commandManager),
	m_Document(document)
{
	m_Line = std::make_shared<Line>(m_CommandManager,m_Document);
	m_Line->elementType = ElementType::Line;
	m_Line->m_QOpenGLFunction = RenderContext::Instance().gl;
	m_Line->m_MatrixModel = RenderContext::Instance().MatrixModel;
	m_Line->m_MatrixView = RenderContext::Instance().MatrixView;
	m_Line->m_MatrixProjection = RenderContext::Instance().MatrixProjection;
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
		m_Line->m_MatrixProjection,
		m_Line->m_MatrixView);
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
		m_Line->m_MatrixProjection,
		m_Line->m_MatrixView);
}
void LineCommand::OnMouseRelease(QMouseEvent* event) {

}
void LineCommand::OnDraw() {
	m_Line->Draw();
}