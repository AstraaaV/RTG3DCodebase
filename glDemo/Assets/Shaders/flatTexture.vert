#version 450 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout (location=0) in vec3 vertexPos;
layout (location=2) in vec2 vertexTexCoord;
layout (location=3) in vec3 vertexNormal;
layout (location=4) in vec3 vertexColour;

out SimplePacket {

  vec3 texCoord;

} outputVertex;


void main(void) {

	outputVertex.texCoord = vertexTexCoord;

	vec4 worldCoord = modelMatrix * vec4(vertexPos, 1.0);

	gl_Position = projMatrix * viewMatrix * worldCoord;
}