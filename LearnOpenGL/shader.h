#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/fwd.hpp>

class Shader
{
public:
	unsigned int Id;
	Shader(const char* vertexPath,const char* fragmentPath);
	void use();
	void del();
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
};
#endif