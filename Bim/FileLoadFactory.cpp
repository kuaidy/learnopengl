#include "FileLoadFactory.h"

std::string FileLoadFactory::ToLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string FileLoadFactory::GetFileExtension(const std::string& fileName) {
    size_t dot = fileName.find_last_of('.');
    if (dot == std::string::npos) return "";
    return ToLower(fileName.substr(dot + 1));
}

std::unique_ptr<FileLoader> FileLoadFactory::Create(const std::string& fileName) {
    std::string ext = GetFileExtension(fileName);

    if (ext == "gltf" || ext == "glb") {
        return std::make_unique<GltfLoader>();
    }
    else if (ext == "ifc") {
        return std::make_unique<IfcLoader>();
    }
    return nullptr;
}