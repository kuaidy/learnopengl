#include "BuildView.h"


BuildView::BuildView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

BuildView::~BuildView()
{}

/// <summary>
/// ªÊ÷∆œﬂ∂Œ
/// </summary>
void BuildView::on_pb_line_triggered() {
	if (command_manager) {
		std::shared_ptr<LineCommand> lineCommand = std::make_shared<LineCommand>();
		command_manager->SetCommand(lineCommand);
 	}
}

