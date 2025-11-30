#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(push_constant) uniform PushConstants {
    mat4 projection;
    mat4 modelview;
} pc;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 inColor;

out gl_PerVertex{
    vec4 gl_Position;
};

layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec4 fragColor;
layout(location = 3) out vec3 fragEyePos;
layout(location = 4) out vec3 fragEyeNormal;

void main() {
    gl_Position = (pc.projection * pc.modelview) * vec4(inPosition, 1.0);
    fragPosition = inPosition;
    fragNormal = inNormal;
    fragColor = inColor;
    fragEyePos=(pc.modelview * vec4(inPosition, 1.0)).xyz;
    mat3 normalMatrix = transpose(inverse(mat3(pc.modelview)));
    fragEyeNormal = normalize(normalMatrix * inNormal);
}