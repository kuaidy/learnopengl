#pragma once
#ifndef FILELOADER_H
#define FILELOADER_H

#include <string>
#include <vector>
#include "element.h"

class FileLoader {
public:
	FileLoader() = default;
	~FileLoader() = default;
	virtual std::vector<Element> Load(const std::string filePath) = 0;
};

#endif // !FILELOADER_H

