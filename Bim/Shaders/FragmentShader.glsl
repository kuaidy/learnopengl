#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform bool isSelected;

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    if (isSelected) {
        // 将高亮颜色混入纹理色，调整混合比例
        float mixFactor = 0.5; // 越大越红
        vec3 color = mix(texColor.rgb, vec3(0.0,0.0,1.0), mixFactor);
        FragColor = vec4(color, texColor.a);
    } else {
        FragColor = texColor;
    }
}