#include "EditView.h"
#include <Commands/AreaMarkCommand.h>
#include <Commands/CommandMode.h>
#include <Commands/CommandState.h>

EditView::EditView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

EditView::~EditView()
{}

/// <summary>
/// 长度标注
/// </summary>
void EditView::on_length_mark_triggered() {

}
/// <summary>
/// 面积标注
/// </summary>
void EditView::on_area_mark_triggered() {
	//auto areaMarkCommand = std::make_shared<AreaMarkCommand>();
	//command_manager->Do(areaMarkCommand);
	//command_manager->commandMode = CommandMode::Line;
	//command_manager->commandState = CommandState::Doing;
}

