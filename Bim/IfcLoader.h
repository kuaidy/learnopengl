#pragma once
#ifndef IFCLOADER_H
#define IFCLOADER_H

#include "FileLoader.h"
#include <string>
class IfcLoader : public FileLoader {
public:
	std::vector<Element> Load(const std::string filePath) override;
};

#endif // !IFCLOADER_H

