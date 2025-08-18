#pragma once
#include <memory>
#include "../Commands/CommandManager.h"

class RectanglePlane : public Element
{
public:
	RectanglePlane(std::shared_ptr<CommandManager> commandmanager, std::shared_ptr<Bim::Document> document);
	~RectanglePlane();
	/// <summary>
	/// 用于实时显示
	/// </summary>
	Bim::Vertex last_vertex;
	void Draw();
private:
	std::shared_ptr<CommandManager> m_command_manager;
	std::shared_ptr<Bim::Document> m_document;
	bool m_initialized = false;
	void Initialize();
	std::vector<QVector3D> GeneratePositions(QVector3D firstpoint,QVector3D secondpoint,QVector3D normal);
};

