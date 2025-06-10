#version 450 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout (location = 0) in vec4 vertexPos;
layout (location = 1) in vec2 vertexUV;
layout (location = 4) in vec4 vertexColour;

out SimplePacket {
    vec3 colour;
    vec2 texCoord;
} outputVertex;

void main(void) {
    outputVertex.colour = vertexColour.rgb;
    outputVertex.texCoord = vertexUV;

    gl_Position = projMatrix * viewMatrix * modelMatrix * vertexPos;
}
