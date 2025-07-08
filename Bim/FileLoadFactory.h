#pragma once
#ifndef FILELOADFACTORY_H
#define FILELOADFACTORY_H

#include "FileLoader.h"
#include "GltfLoader.h"
#include "IfcLoader.h"

#include <string>
#include <algorithm>
#include <memory>

class FileLoadFactory {
public:
	static std::string ToLower(const std::string& str);
	static std::string GetFileExtension(const std::string& fileName);
	static std::unique_ptr<FileLoader> Create(const std::string& fileName);
};
#endif // FILELOADFACTORY_H

