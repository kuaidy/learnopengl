#include "Document.h"
/// <summary>
/// Ìí¼ÓÔªËØ
/// </summary>
/// <param name="element"></param>
void Bim::Document::AddElement(std::shared_ptr<Element> element) {
	if (element)
		element->mesh.SetupMesh();
	elements.push_back(element);
}
/// <summary>
/// äÖÈ¾
/// </summary>
void Bim::Document::DrawElements() {
	for (int i = 0; i < elements.size(); ++i) {
		elements[i]->mesh.DrawByType(*(elements[i]->shader),elements[i]->element_type);
	}
}