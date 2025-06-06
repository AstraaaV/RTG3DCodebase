#version 450 core

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec2 in_TexCoord;
layout(location = 2) in vec3 in_Normal;

layout(location = 0) out vec2 TexCoord;
layout(location = 1) out vec3 FragPos;
layout(location = 2) out vec3 Normal;

layout(std140, binding = 0) uniform Matrices {
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 projMatrix;
};

void main()
{
    FragPos = vec3(modelMatrix * in_Position);
    Normal = mat3(transpose(inverse(modelMatrix))) * in_Normal;
    TexCoord = in_TexCoord;
    gl_Position = projMatrix * viewMatrix * vec4(FragPos, 1.0);
}