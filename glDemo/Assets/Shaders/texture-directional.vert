#version 450 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexNormal;
layout (location=2) in vec2 vertexTexCoord;

out SimplePacket {

  vec3 surfaceWorldPos;
  vec3 surfaceNormal;
  vec2 texCoord;

} vertexOut;


void main(void) {

  vertexOut.texCoord = vertexTexCoord;

  // transform normal vector by inverse-transpose of the model matrix
  vertexOut.surfaceNormal = (transpose(inverse(modelMatrix)) * vec4(vertexNormal, 0.0)).xyz;

  // take vertexPos into world coords and pass onto fragment shader
  vec4 worldPos = modelMatrix * vec4(vertexPos, 1.0);
  vertexOut.surfaceWorldPos = worldPos.xyz; // don't need w element

  // take worldCoord rest of the way into clip coords and set in gl_Position
	gl_Position = projMatrix * viewMatrix * worldPos;
}
