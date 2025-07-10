#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

//uniform sampler2D texture_diffuse1;
//uniform bool isSelected;

void main()
{
    //FragColor = texture(texture_diffuse1, TexCoords);
    FragColor = vec4(0.8, 0.3, 0.2, 1.0); // äÖÈ¾Îª¹Ì¶¨ÑÕÉ«
}