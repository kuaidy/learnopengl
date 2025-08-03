#pragma once
#include <memory>
#include "../Commands/CommandManager.h"
#include "../Document.h"
#include "../Element.h"
#include "../Mesh.h"

class Ball:public Element
{
public:
	Ball(std::shared_ptr<CommandManager> ball_command_manager, 
		std::shared_ptr<Bim::Document> ball_document);
	~Ball();
	void Draw();

	int stacks = 32;
	int slices = 32;
	float radius = 1.0f;

private:
	std::shared_ptr<CommandManager> m_command_manager;
	std::shared_ptr<Bim::Document> m_document;
	bool m_initialized = false;
	void Initialize();
};

