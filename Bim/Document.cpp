#include "Document.h"
/// <summary>
/// Ìí¼ÓÔªËØ
/// </summary>
/// <param name="element"></param>
void Bim::Document::AddElement(std::shared_ptr<Element> element) {
	if (element)
		element->mesh.SetupMesh();
	m_Elements.push_back(element);
}
/// <summary>
/// äÖÈ¾
/// </summary>
void Bim::Document::DrawElements() {
	for (int i = 0; i < m_Elements.size(); ++i) {
		m_Elements[i]->mesh.DrawByType(*(m_Elements[i]->m_Shader),m_Elements[i]->elementType);
	}
}