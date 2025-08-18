#include "BuildView.h"



BuildView::BuildView(QWidget* parent,
	std::shared_ptr<CommandManager> commandManager, 
	std::shared_ptr<Bim::Document> document)
	: QWidget(parent),
	m_CommandManager(commandManager),
	m_Document(document)
{
	ui.setupUi(this);
}

BuildView::~BuildView()
{
}

/// <summary>
/// 绘制线段
/// </summary>
void BuildView::on_pbline_clicked() {
	if (m_CommandManager) {
		std::shared_ptr<LineCommand> lineCommand = std::make_shared<LineCommand>(m_CommandManager, m_Document);
		m_CommandManager->SetCommand(lineCommand);
	}
}
/// <summary>
/// 绘制球
/// </summary>
void BuildView::on_pbball_clicked() {
	if (m_CommandManager) {
		std::shared_ptr<BallCommand> ballcommand = std::make_shared<BallCommand>(m_CommandManager, m_Document);
		m_CommandManager->SetCommand(ballcommand);
	}
}
/// <summary>
/// 绘制矩形平面
/// </summary>
void BuildView::on_pbrectangleplane_clicked() {
	if (m_CommandManager) {
		std::shared_ptr<RectanglePlaneCommand> rectangleplanecommand = std::make_shared<RectanglePlaneCommand>(m_CommandManager, m_Document);
		m_CommandManager->SetCommand(rectangleplanecommand);
	}
}

