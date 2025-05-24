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

//��ͼ����
uniform vec3 viewPos;

void main(){
    //�����⣬���ú�������һ����ֵ
    vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;

    //������
    //1.��������׼��
    vec3 norm=normalize(Normal);
    //2.���շ���
    vec3 lightDir=normalize(light.position-FragPos);
    //3.����������ϵ��
    float diff=max(dot(norm,lightDir),0.0f);
    //4.��ȡ����Ƭ���е���������ɫֵ
    vec3 diffuse=light.diffuse * diff * texture(material.diffuse,TexCoords).rgb;

    //���淴��
    //1.����۲췽��
    vec3 viewDir=normalize(viewPos-FragPos);
    //2.���㷴�䷽��
    vec3 reflectDir=reflect(-lightDir,norm);
    //3.������ڹ۲췽���ϵ�ǿ��,32���ݣ���ɢ���ǿ�ȣ�Խ�����Խ����
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    //4.��ȡ����Ƭ�εľ��淴����ɫֵ
    vec3 specular=light.specular * spec * texture(material.specular,TexCoords).rgb;
    
    //�������ͼ
    vec3 emission=texture(material.emission,TexCoords).rgb;

    vec3 result=(ambient+diffuse+specular+emission);
    FragColor=vec4(result,1.0f);
}