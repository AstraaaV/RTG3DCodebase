#version 450 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout (location=0) in vec3 vertexPos;
layout (location=2) in vec3 vertexTexCoord;
layout (location=3) in vec3 vertexNormal;

out SimplePacket {

  vec3 surfaceWorldPos;
  vec3 surfaceNormal;
  vec2 texCoord;

} outputVertex;


void main(void) {

  outputVertex.texCoord = vertexTexCoord.st;
  outputVertex.surfaceNormal = (transpose(inverse(modelMatrix)) * vec4(vertexNormal, 0.0)).xyz;
  vec4 worldCoord = modelMatrix * vec4(vertexPos, 1.0);
  outputVertex.surfaceWorldPos = worldCoord.xyz;

  gl_Position = projMatrix * viewMatrix * worldCoord;
}
