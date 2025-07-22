#pragma once

#include <QWidget>
#include "ui_EditView.h"
#include <Commands/CommandManager.h>

class EditView : public QWidget
{
	Q_OBJECT

public:
	EditView(QWidget *parent = nullptr);
	~EditView();
	std::shared_ptr<CommandManager> command_manager;
protected slots:
	void on_length_mark_triggered();
	void on_area_mark_triggered();

private:
	Ui::EditViewClass ui;
};

