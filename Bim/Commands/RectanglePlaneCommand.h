#pragma once
#include "ICommand.h"
#include "CommandManager.h"
#include "../Builds/RectanglePlane.h"
#include "../Commons/Transform.h"

class RectanglePlaneCommand:public ICommand
{
public:
	RectanglePlaneCommand(std::shared_ptr<CommandManager> commandManager, std::shared_ptr<Bim::Document> document);
	void Execute() override;
	void Finish(std::shared_ptr <Bim::Document> document) override;
	void Cancel() override;
	void OnMousePress(QMouseEvent* event) override;
	void OnMouseMove(QMouseEvent* event) override;
	void OnMouseRelease(QMouseEvent* event) override;
	void OnDraw() override;
private:
	std::shared_ptr<RectanglePlane> m_rectangle_plane;
	std::shared_ptr<CommandManager> m_command_manager;
	std::shared_ptr<Bim::Document> m_document;
};