#version 450 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 vertexTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec2 texCoord;

void main()
{
    texCoord = vertexTexCoord;
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(vertexPos, 1.0);
}