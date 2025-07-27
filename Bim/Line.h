#pragma once
#include "Element.h"
class Line :public Element
{
public:
	Line();
	~Line();
	/// <summary>
	/// 用于实时显示线段
	/// </summary>
	Bim::Vertex last_vertex;
	void Draw();
private:
	unsigned int vao;
	unsigned int vbo;
};

