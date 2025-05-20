#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

uniform vec3 lightColor;
uniform vec3 objectColor;
//uniform vec3 lightPos;
//uniform vec3 viewPos;

void main(){
    //��������
    float ambientStrength=0.1;
    vec3 ambient=ambientStrength*lightColor;
    //FragColor=vec4(ambient*objectColor,1.0f);
    
    //������
    //��������׼��
    vec3 norm=normalize(Normal);
    //��Դ�����׼��
    //vec3 lightDir=normalize(lightPos-FragPos);
    vec3 lightDir=normalize(LightPos-FragPos);
    float diff=max(dot(norm,lightDir),0.0);
    vec3 diffuse=diff*lightColor;
    //vec3 result=(ambient + diffuse)*objectColor;

    //����ǿ��
    float specularStength=0.5;
    //vec3 viewDir=normalize(viewPos-FragPos);
    vec3 viewDir=normalize(-FragPos);
    //���䷽��
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),256);
    vec3 specular=specularStength*spec*lightColor;

    vec3 result=(ambient+diffuse+specular)*objectColor;

    FragColor=vec4(result,1.0f);
}