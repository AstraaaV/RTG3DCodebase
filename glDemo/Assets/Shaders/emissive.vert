#version 450 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

out vec2 texCoord;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(inPosition, 1.0);
    texCoord = inTexCoord;
}
