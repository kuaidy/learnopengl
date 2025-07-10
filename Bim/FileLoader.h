#pragma once
#ifndef FILELOADER_H
#define FILELOADER_H

#include <string>
#include <vector>
#include "element.h"
#include "scenetree.h"
#include "ElementProperty.h"

class FileLoader {
public:
	FileLoader() = default;
	~FileLoader() = default;
	virtual bool Load(const std::string filePath) = 0;

	std::vector<Element> elements;
	SceneTree scene_tree;
	std::vector<ElementProperty> element_properties;
};

#endif // !FILELOADER_H

