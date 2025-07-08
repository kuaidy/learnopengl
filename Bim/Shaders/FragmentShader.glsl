#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
//uniform bool isSelected;

void main()
{
    vec4 FragColor = texture(texture_diffuse1, TexCoords);
}