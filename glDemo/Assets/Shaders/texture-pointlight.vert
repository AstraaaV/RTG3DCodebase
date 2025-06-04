#version 450 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexNormal;

out SimplePacket {
    vec3 surfaceWorldPos;
    vec3 surfaceNormal;
    vec2 texCoord;
} outputVertex;

void main(void) {
    outputVertex.texCoord = vertexTexCoord.st;

    // Transform normal into world space
    outputVertex.surfaceNormal = (transpose(inverse(modelMatrix)) * vec4(vertexNormal, 0.0)).xyz;

    // Calculate world space position of vertex
    vec4 worldCoord = modelMatrix * vec4(vertexPos, 1.0);
    outputVertex.surfaceWorldPos = worldCoord.xyz;

    // Final position in clip space
    gl_Position = projMatrix * viewMatrix * worldCoord;
}
