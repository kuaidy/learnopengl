#version 460 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

out vec3 Normal;
//计算世界坐标
out vec3 FragPos;
out vec3 LightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

void main(){
	gl_Position=projection*view*model*vec4(aPos,1.0f);
	//法线矩阵
	Normal=mat3(transpose(inverse(view*model)))*aNormal;
	FragPos=vec3(view*model*vec4(aPos,1.0f));
	LightPos=vec3(view*vec4(lightPos,1.0f));
}