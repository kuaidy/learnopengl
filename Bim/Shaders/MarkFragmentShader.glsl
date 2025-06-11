#version 460 core
out vec4 FragColor;

uniform vec3 FontColor

void main()
{
    FragColor = vec4(FontColor,1.0);
}