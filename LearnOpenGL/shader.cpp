#include "shader.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath){
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		//转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure) {
		std::cout << "file read error" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	//编译着色器
	unsigned int vertex, fragment;
	int success;
	char infolog[512];

	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex,512,NULL,infolog);
		std::cout << "failed compile vertex shader"<< infolog << std::endl;
	}
	//片段着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment,GL_COMPILE_STATUS,&success);
	if (!success) {
		glGetShaderInfoLog(fragment,512,NULL,infolog);
		std::cout << "failed compile fragment shader" << std::endl;
	}
	//着色器程序
	Id = glCreateProgram();
	glAttachShader(Id, vertex);
	glAttachShader(Id,fragment);
	glLinkProgram(Id);
	glGetProgramiv(Id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(Id,512,NULL,infolog);
		std::cout << "failed link program" << std::endl;
	}
	//链接之后，可以删除着色器
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
void Shader::use() {
	glUseProgram(Id);
}
void Shader::del() 
{
	glDeleteProgram(Id);
}
//uniform设置
void Shader::setFloat(const std::string &name,float value) const {
	glUniform1f(glGetUniformLocation(Id,name.c_str()),value);
}
/// <summary>
/// 矩阵
/// </summary>
/// <param name="name"></param>
/// <param name="value"></param>
void Shader::setMat4(const std::string &name,glm::mat4 value) const 
{
	glUniformMatrix4fv(glGetUniformLocation(Id, name.c_str()),1,GL_FALSE,glm::value_ptr(value));
}