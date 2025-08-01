#pragma once
#include <vector>
#include <Element.h>
namespace Bim {
	class Document
	{
	public:
		void AddElement(std::shared_ptr<Element> element);
		void DrawElements();
	private:
		std::vector<std::shared_ptr<Element>> m_Elements;
	};
}