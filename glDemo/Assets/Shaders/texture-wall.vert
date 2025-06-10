#version 450 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout (location=0) in vec3 vertexPos;
layout (location=2) in vec2 vertexTexCoord;
layout (location=3) in vec3 vertexNormal;
layout (location=4) in vec3 vertexTangent;
layout (location=5) in vec3 vertexBitangent;

out SimplePacket {
	vec3 surfaceWorldPos;
	vec3 surfaceNormal;
	vec2 texCoord;
	mat3 TBN;
} outputVertex;

void main(void) {
	outputVertex.texCoord = vertexTexCoord.st;

	vec3 T = normalize(mat3(modelMatrix) * vertexTangent);
	vec3 N = normalize(mat3(transpose(inverse(modelMatrix))) * vertexNormal);
	vec3 B = cross(N, T);

	outputVertex.TBN = mat3(T, B, N);
	outputVertex.surfaceNormal = N;

	vec4 worldCoord = modelMatrix * vec4(vertexPos, 1.0);
	outputVertex.surfaceWorldPos = worldCoord.xyz;

	gl_Position = projMatrix * viewMatrix * worldCoord;
}
