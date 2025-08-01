#pragma once

#include <QWidget>
#include "ui_BuildView.h"
#include "Commands/LineCommand.h"
#include "Commands/CommandManager.h"

class BuildView : public QWidget
{
	Q_OBJECT

public:
	BuildView(QWidget* parent = nullptr,
		std::shared_ptr<CommandManager> commandManager = nullptr,
		std::shared_ptr<Bim::Document> document = nullptr);
	~BuildView();

public slots:
	void on_pbline_clicked();
private:
	Ui::BuildViewClass ui;
	std::shared_ptr<CommandManager> m_CommandManager;
	std::shared_ptr<Bim::Document> m_Document;
};

