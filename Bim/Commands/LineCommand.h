#pragma once
#include "ICommand.h"
#include "../Builds/Line.h"
#include "../Commons/Transform.h"
#include "../Commons/RenderContext.h"
#include "../Document.h"
#include "../Mesh.h"
#include "CommandManager.h"

class LineCommand :public ICommand
{
public:
	LineCommand(std::shared_ptr<CommandManager> commandManager, std::shared_ptr<Bim::Document> document);
	void Execute() override;
	void Finish(std::shared_ptr <Bim::Document> document) override;
	void Cancel() override;
	void OnMousePress(QMouseEvent* event) override;
	void OnMouseMove(QMouseEvent* event) override;
	void OnMouseRelease(QMouseEvent* event) override;
	void OnDraw() override;
private:
	std::shared_ptr<Line> m_Line;
	std::shared_ptr<CommandManager> m_CommandManager;
	std::shared_ptr<Bim::Document> m_Document;
};

