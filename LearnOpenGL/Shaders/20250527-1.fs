#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
//������
in vec3 Normal;
//��������
in vec3 FragPos;

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    //sampler2D emission;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    vec3 direction;//����⣬����
    float cutOff;//�й��
    float outerCutOff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

};
uniform Light light;

//��ͼ����
uniform vec3 viewPos;

//�����
struct DirLight{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir){
    vec3 lightDir=normalize(-light.direction);
    //��������ɫ
    float diff=max(dot(normal,lightDir),0.0);
    //�������ɫ
    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    //�ϲ����
    vec3 ambient=light.ambient* vec3(texture(material.diffuse,TexCoords));
    vec3 diffuse=light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
    vec3 specular=light.specular * spec * vec3(texture(material.specular,TexCoords));
    return (ambient+diffuse+specular);
};

//���Դ
struct PointLight
{
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir){
    vec3 lightDir=normalize(light.position-fragPos);
    //��������ɫ
    float diff=max(dot(normal,lightDir),0.0);
    //�������ɫ
    vec3 reflecDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(viewDir,reflecDir),0.0),material.shininess);
    //˥��
    float distance=length(light.position-fragPos);
    float attenuation=1.0/(light.constant+light.linear * distance + light.quadratic * (distance * distance));
    vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
    vec3 specular= light.specular * spec * vec3(texture(material.specular,TexCoords));
    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;
    return (ambient+diffuse+specular);
};

//�۹�
struct SpotLight{
    vec3 position;
    vec3 direction;
    float cutOff;//�й��
    float outerCutOff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
unifrom SpotLight spotLight;

vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir=normalize(light.position-fragPos);
    float theta=dot(lightDir,normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
    vec3 specular= light.specular * spec * vec3(texture(material.specular,TexCoords));
    diffuse*=intensity;
    specular*=intensity;
    return (ambient+diffuse+specular);
}

void main(){
    //����
    vec3 norm = normalize(Normal);
    vec3 viewDir=normalize(viewPos-FragPos);
    //��һ�׶�:�������
    vec3 result=CalcDirLight(dirLight,norm,viewDir);
    //�ڶ��׶Σ����Դ
    for(int i=0;i<NR_POINT_LIGHTS;i++){
        result+=CalcPointLight(pointLights[i],norm,FragPos,viewDir);
    }
    //�����׶Σ��۹�
    result += CalcSpotLight(spotLight,norm,FragPos,viewDir);

    FragColor=vec4(result,1.0);

    ////�����⣬���ú�������һ����ֵ
    //vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;

    ////������
    ////1.��������׼��
    //vec3 norm=normalize(Normal);
    ////2.���շ���
    //vec3 lightDir=normalize(light.position-FragPos);
    ////vec3 lightDir=normalize(-light.direction);
    ////3.����������ϵ��
    //float diff=max(dot(norm,lightDir),0.0f);
    ////4.��ȡ����Ƭ���е���������ɫֵ
    //vec3 diffuse=light.diffuse * diff * texture(material.diffuse,TexCoords).rgb;

    ////���淴��
    ////1.����۲췽��
    //vec3 viewDir=normalize(viewPos-FragPos);
    ////2.���㷴�䷽��
    //vec3 reflectDir=reflect(-lightDir,norm);
    ////3.������ڹ۲췽���ϵ�ǿ��,32���ݣ���ɢ���ǿ�ȣ�Խ�����Խ����
    //float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    ////4.��ȡ����Ƭ�εľ��淴����ɫֵ
    //vec3 specular=light.specular * spec * texture(material.specular,TexCoords).rgb;
    
    ////�������ͼ
    ////vec3 emission=texture(material.emission,TexCoords).rgb;

    ////vec3 result=(ambient+diffuse+specular+emission);

    ////˥��
    ////float distance=length(light.position-FragPos);
    ////float attenuation=1.0/(light.constant+light.linear*distance+light.quadratic*(distance*distance));
    ////ambient  *= attenuation; 
    ////diffuse  *= attenuation;
    ////specular *= attenuation;

    ////�۹�
    //float theta=dot(lightDir,normalize(-light.direction));
    //float epsilon   = light.cutOff - light.outerCutOff;
    //float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    
    ////if(theta>light.cutOff){
    ////    //ִ�й��ռ���
    ////    FragColor=vec4(ambient+diffuse+specular,1.0f);
    ////}
    ////else{
    ////    //����ʹ�û����⣬�ó����ھ۹�֮��ʱ��������ȫ�ڰ�
    ////    FragColor=vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
    ////}
    //diffuse*=intensity;
    //specular*=intensity;
    //FragColor=vec4(ambient+diffuse+specular,1.0f);
}