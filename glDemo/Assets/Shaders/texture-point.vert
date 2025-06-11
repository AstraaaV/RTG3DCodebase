#version 450 core

// ── Matrices supplied by the engine ────────────────────────────────────────
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

// ── Vertex attributes (match VAO layout) ───────────────────────────────────
layout(location = 0) in vec3 vertexPos;
layout(location = 2) in vec2 vertexTexCoord;   // you used vec3 in the old file, but only .st is needed
layout(location = 3) in vec3 vertexNormal;

// ── Packet passed to the fragment stage ────────────────────────────────────
out SimplePacket {
    vec3 surfaceWorldPos;
    vec3 surfaceNormal;
    vec2 texCoord;
} vsOut;

void main()
{
    // Texture coordinates
    vsOut.texCoord = vertexTexCoord.st;

    // Transform normal by inverse-transpose(model) to keep it orthogonal
    vsOut.surfaceNormal =
        mat3(transpose(inverse(modelMatrix))) * vertexNormal;

    // World-space position
    vec4 worldPos = modelMatrix * vec4(vertexPos, 1.0);
    vsOut.surfaceWorldPos = worldPos.xyz;

    // Standard MVP
    gl_Position = projMatrix * viewMatrix * worldPos;
}
