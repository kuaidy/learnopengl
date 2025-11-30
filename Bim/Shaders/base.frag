#version 450
layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec4 fragColor;

layout(location = 3) in vec3 fragEyePos;
layout(location = 4) in vec3 fragEyeNormal;

layout(location = 0) out vec4 outColor;

void main() {
    // 环境光
    vec3 ambient = 0.3 * fragColor.rgb;
    
    // 方向光
    vec3 lightDir = normalize(-fragEyePos);
    vec3 normal = normalize(fragEyeNormal);
    
    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = 0.7 * diff * fragColor.rgb;
    
    // 镜面反射
    vec3 viewDir = normalize(-fragEyePos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = 0.3 * spec * vec3(1.0, 1.0, 1.0);
    
    vec3 result = ambient + diffuse + specular;
    outColor = vec4(result, fragColor.a);
}