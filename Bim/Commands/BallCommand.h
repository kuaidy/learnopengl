#pragma once
#include "ICommand.h"
#include "CommandManager.h"

#include "../Builds/Ball.h"

class BallCommand :public ICommand
{
public:
	BallCommand(std::shared_ptr<CommandManager> commandmanager,
		std::shared_ptr<Bim::Document> document);
	 void Execute() override;
	 void Finish(std::shared_ptr<Bim::Document> document) override;
	 void Cancel() override;
	 void OnMousePress(QMouseEvent* event) override;
	 void OnMouseMove(QMouseEvent* event) override;
	 void OnMouseRelease(QMouseEvent* event) override;
	 void OnDraw() override;
private:
	std::shared_ptr<Ball> m_ball;
	std::shared_ptr<CommandManager> m_command_manager;
	std::shared_ptr<Bim::Document> m_document;
};

