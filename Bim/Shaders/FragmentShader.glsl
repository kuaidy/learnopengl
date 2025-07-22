#version 460 core
out vec4 FragColor;

in vec4 fragWorldPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform bool is_selected;
uniform vec4 u_clipPlane;

void main()
{
    float distance = dot(vec3(u_clipPlane),fragWorldPos.xyz)+u_clipPlane.w;
    if(distance>0.0){
        discard;
    }
    vec4 baseColor = texture(texture_diffuse1, TexCoords);
    if(is_selected){
        FragColor = mix(baseColor, vec4(0.0, 0.0, 1.0, 1.0),0.2);
    }
    else{
        FragColor=baseColor;
    }
    //FragColor = texture(texture_diffuse1, TexCoords);
    //FragColor = vec4(0.8, 0.3, 0.2, 1.0);
}