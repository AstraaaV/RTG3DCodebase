#version 450 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout (location = 0) in vec3 vertexPos;
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in vec3 vertexNormal;

out vec2 fragUV;
out vec3 fragNormal;
out vec3 fragWorldPos;

void main()
{
	vec4 worldPos = modelMatrix * vec4(vertexPos, 1.0);
	fragWorldPos = worldPos.xyz;

	fragNormal = normalize(mat3(modelMatrix) * vertexNormal);
	fragUV = vertexTexCoord;

	gl_Position = projMatrix * viewMatrix * worldPos;
}
