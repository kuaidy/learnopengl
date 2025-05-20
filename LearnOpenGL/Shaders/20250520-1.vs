#version 460 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

//������
out vec3 Normal;
//Ƭ������
out vec3 FragPos;
//��Դ����
out vec3 LightPos;

//ģ�;���
uniform mat4 model;
//��ͼ����
uniform mat4 view;
//ͶӰ����
uniform mat4 projection;
//��Դ����
uniform vec3 lightPos;

void main(){
	gl_Position=projection*view*model*vec4(aPos,1.0f);
	//���߾���
	Normal=mat3(transpose(inverse(view*model)))*aNormal;
	FragPos=vec3(view*model*vec4(aPos,1.0f));
	LightPos=vec3(view*vec4(lightPos,1.0f));
}