#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;


uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main(){
    //环境光照
    vec3 ambient=light.ambient*material.ambient;
    
    //漫反射
    //法向量标准化
    vec3 norm=normalize(Normal);
    //光源方向标准化
    vec3 lightDir=normalize(LightPos-FragPos);
    float diff=max(dot(norm,lightDir),0.0);
    vec3 diffuse=light.diffuse*(diff*material.diffuse);

    //镜面光
    vec3 viewDir=normalize(viewPos-FragPos);
    //反射方向
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular=light.specular*(spec*material.specular);

    vec3 result=ambient+diffuse+specular;
    FragColor=vec4(result,1.0f);
}