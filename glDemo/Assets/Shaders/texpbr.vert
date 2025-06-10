#version 450 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout(location = 0) in vec3 vertexPos;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexNormal;
layout(location = 4) in vec3 vertexTangent; // Add tangent attribute support for normal maps

out SimplePacket {
    vec3 surfaceWorldPos;
    vec3 surfaceNormal;
    vec2 texCoord;
    mat3 TBN;
} outputVertex;

void main(void)
{
    // Transform position to world space
    vec4 worldPos = modelMatrix * vec4(vertexPos, 1.0);
    outputVertex.surfaceWorldPos = worldPos.xyz;
    outputVertex.texCoord = vertexTexCoord;

    // Compute TBN matrix for normal mapping
    vec3 T = normalize(mat3(modelMatrix) * vertexTangent);
    vec3 N = normalize(mat3(transpose(inverse(modelMatrix))) * vertexNormal);
    vec3 B = cross(N, T);
    outputVertex.TBN = mat3(T, B, N);

    // Final position for rendering
    gl_Position = projMatrix * viewMatrix * worldPos;
}
