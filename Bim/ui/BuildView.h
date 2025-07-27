#pragma once

#include <QWidget>
#include "ui_BuildView.h"
#include "Commands/LineCommand.h"
#include "Commands/CommandManager.h"

class BuildView : public QWidget
{
	Q_OBJECT

public:
	BuildView(QWidget *parent = nullptr);
	~BuildView();
	std::shared_ptr<CommandManager> command_manager;
protected slots:
	void on_pb_line_triggered();
private:
	Ui::BuildViewClass ui;
};

