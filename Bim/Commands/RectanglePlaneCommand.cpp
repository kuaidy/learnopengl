#include "RectanglePlaneCommand.h"
RectanglePlaneCommand::RectanglePlaneCommand(std::shared_ptr<CommandManager> commandmanager, std::shared_ptr<Bim::Document> document)
	:m_command_manager(commandmanager), m_document(document)
{
	m_rectangle_plane = std::make_shared<RectanglePlane>(m_command_manager, m_document);
	m_rectangle_plane->element_type = ElementType::Plane;
}
void RectanglePlaneCommand::Execute()
{

};
void RectanglePlaneCommand::Finish(std::shared_ptr <Bim::Document> document) {

}
void RectanglePlaneCommand::Cancel() {

}
void RectanglePlaneCommand::OnMousePress(QMouseEvent* event) {
	QVector3D p = Transform::ScreenToWorld(
		event->pos().x(),
		event->pos().y(),
		RenderContext::Instance().width,
		RenderContext::Instance().height,
		RenderContext::Instance().matrix_projection,
		RenderContext::Instance().matrix_view);
	Bim::Vertex vertex;
	vertex.Position = p;
	m_rectangle_plane->mesh.vertices.push_back(vertex);
}
void RectanglePlaneCommand::OnMouseMove(QMouseEvent* event) {
	m_rectangle_plane->last_vertex.Position = Transform::ScreenToWorld(
		event->pos().x(),
		event->pos().y(),
		RenderContext::Instance().width,
		RenderContext::Instance().height,
		RenderContext::Instance().matrix_projection,
		RenderContext::Instance().matrix_view);
}
void RectanglePlaneCommand::OnMouseRelease(QMouseEvent* event) {

}
void RectanglePlaneCommand::OnDraw() {
	m_rectangle_plane->Draw();
}