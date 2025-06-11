#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D texture_diffuse1;

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    // 应用高亮：可以简单地通过增加亮度或混合特定的颜色来实现高亮
    // 这里我们使用highlightColor和highlightIntensity来控制高亮效果
    vec3 finalColor = texColor.rgb + vec3(0.0,0.0,1.0) * 0.5;
    // 确保颜色不会超过1.0
    finalColor = clamp(finalColor, 0.0, 1.0);
    FragColor = vec4(finalColor, texColor.a);
}