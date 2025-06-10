#version 450 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout(location = 0) in vec3 vertexPos;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexNormal;
layout(location = 4) in vec3 vertexTangent;
layout(location = 5) in vec3 vertexBitangent;

out SimplePacket {
    vec3 surfaceWorldPos;
    vec3 surfaceNormal;
    vec2 texCoord;
    mat3 TBN;
} outputVertex;

void main()
{
    // Pass world position
    vec4 worldPos = modelMatrix * vec4(vertexPos, 1.0);
    outputVertex.surfaceWorldPos = worldPos.xyz;

    // Pass texture coordinates
    outputVertex.texCoord = vertexTexCoord * vec2(0.3, 0.3);

    // Compute TBN matrix (tangent space basis)
    mat3 normalMatrix = mat3(modelMatrix);
    vec3 T = normalize(normalMatrix * vertexTangent);
    vec3 B = normalize(normalMatrix * vertexBitangent);
    vec3 N = normalize(normalMatrix * vertexNormal);
    outputVertex.surfaceNormal = N;
    outputVertex.TBN = mat3(T, B, N);

    // Output final clip space position
    gl_Position = projMatrix * viewMatrix * worldPos;
}
