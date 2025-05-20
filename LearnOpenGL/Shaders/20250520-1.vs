#version 460 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

//法向量
out vec3 Normal;
//片段坐标
out vec3 FragPos;
//光源坐标
out vec3 LightPos;

//模型矩阵
uniform mat4 model;
//视图矩阵
uniform mat4 view;
//投影矩阵
uniform mat4 projection;
//光源坐标
uniform vec3 lightPos;

void main(){
	gl_Position=projection*view*model*vec4(aPos,1.0f);
	//法线矩阵
	Normal=mat3(transpose(inverse(view*model)))*aNormal;
	FragPos=vec3(view*model*vec4(aPos,1.0f));
	LightPos=vec3(view*vec4(lightPos,1.0f));
}