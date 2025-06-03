#version 450 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout (location=0) in vec4 vertexPos;
layout (location=4) in vec4 vertexColour;

out SimplePacket {

  vec3 colour;

} outputVertex;


void main(void) {

	outputVertex.colour = vertexColour.rgb;

	gl_Position = projMatrix * viewMatrix * modelMatrix * vertexPos;
}
