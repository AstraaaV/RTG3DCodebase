#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoordInput;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec2 texCoord;

void main()
{
    texCoord = texCoordInput;
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}