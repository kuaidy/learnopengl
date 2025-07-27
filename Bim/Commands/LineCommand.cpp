#include "LineCommand.h"
#include "../Commons/Transform.h"
#include "../Mesh.h"

LineCommand::LineCommand() {
	m_Line = std::make_shared<Line>();
}

void LineCommand::Execute() {

}
void LineCommand::Finish() {

}
void LineCommand::Cancel() {

}
void LineCommand::OnMousePress(QMouseEvent* event) {
	QVector3D p = Transform::ScreenToWorld(
		event->pos().x(),
		event->pos().y(),
		m_Line->width,
		m_Line->height,
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
		m_Line->width,
		m_Line->height,
		m_Line->m_MatrixProjection,
		m_Line->m_MatrixView);
}
void LineCommand::OnMouseRelease(QMouseEvent* event) {

}
void LineCommand::OnDraw() {
	m_Line->Draw();
}