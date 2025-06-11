#version 450 core

layout(location = 0) in vec4 vertexPos;
layout(location = 1) in vec2 vertexUV;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec2 fragUV;

void main(void) {
    fragUV = vertexUV;
    gl_Position = projMatrix * viewMatrix * modelMatrix * vertexPos;
}
