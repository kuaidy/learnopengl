#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D texture_diffuse1;

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    // Ӧ�ø��������Լ򵥵�ͨ���������Ȼ����ض�����ɫ��ʵ�ָ���
    // ��������ʹ��highlightColor��highlightIntensity�����Ƹ���Ч��
    vec3 finalColor = texColor.rgb + vec3(0.0,0.0,1.0) * 0.5;
    // ȷ����ɫ���ᳬ��1.0
    finalColor = clamp(finalColor, 0.0, 1.0);
    FragColor = vec4(finalColor, texColor.a);
}