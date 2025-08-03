#pragma once
#include "Element.h"
#include "Commands/CommandManager.h"
#include "Document.h"

class Line :public Element
{
public:
	Line(std::shared_ptr<CommandManager> line_command_manager,std::shared_ptr<Bim::Document> line_document);
	~Line();
	/// <summary>
	/// 用于实时显示线段
	/// </summary>
	Bim::Vertex last_vertex;
	void Draw();
private:
	std::shared_ptr<CommandManager> m_command_manager;
	std::shared_ptr<Bim::Document> m_document;
	bool m_initialized = false;
	void Initialize();
};

