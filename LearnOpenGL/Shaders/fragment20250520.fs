#version 460 core
out vec4 FragColor;


//uniform vec3 lightPos;
//uniform vec3 viewPos;

in vec3 LightingColor;

uniform vec3 objectColor;

void main(){
    FragColor=vec4(LightingColor*objectColor,1.0f);
}