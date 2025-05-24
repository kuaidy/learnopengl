#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
//法向量
in vec3 Normal;
//顶点坐标
in vec3 FragPos;

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
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

//视图方向
uniform vec3 viewPos;

void main(){
    //环境光，设置和漫反射一样的值
    vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;

    //漫反射
    //1.法向量标准化
    vec3 norm=normalize(Normal);
    //2.光照方向
    vec3 lightDir=normalize(light.position-FragPos);
    //3.计算慢反射系数
    float diff=max(dot(norm,lightDir),0.0f);
    //4.获取纹理片段中的漫反射颜色值
    vec3 diffuse=light.diffuse * diff * texture(material.diffuse,TexCoords).rgb;

    //镜面反射
    //1.计算观察方向
    vec3 viewDir=normalize(viewPos-FragPos);
    //2.计算反射方向
    vec3 reflectDir=reflect(-lightDir,norm);
    //3.反射光在观察方向上的强度,32次幂，是散射的强度，越大光线越集中
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    //4.获取纹理片段的镜面反射颜色值
    vec3 specular=light.specular * spec * texture(material.specular,TexCoords).rgb;
    
    //放射光贴图
    vec3 emission=texture(material.emission,TexCoords).rgb;

    vec3 result=(ambient+diffuse+specular+emission);
    FragColor=vec4(result,1.0f);
}