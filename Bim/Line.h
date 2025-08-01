#pragma once
#include "Element.h"
#include "Commands/CommandManager.h"
#include "Document.h"

class Line :public Element
{
public:
	Line(std::shared_ptr<CommandManager> commandManager,std::shared_ptr<Bim::Document> document);
	~Line();
	/// <summary>
	/// 用于实时显示线段
	/// </summary>
	Bim::Vertex last_vertex;
	void Draw();
private:
	unsigned int vao;
	unsigned int vbo;
	std::shared_ptr<CommandManager> m_CommandManager;
	std::shared_ptr<Bim::Document> m_Document;
};

