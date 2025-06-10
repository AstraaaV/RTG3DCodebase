#version 450 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout(location = 0) in vec3 vertexPos;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexNormal;
layout(location = 4) in vec3 vertexTangent;  // Tangent needed for normal mapping

out VS_OUT {
    vec3 fragPos;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    float alpha;
} vs_out;

void main() {
    vs_out.texCoord = vertexTexCoord;
    vs.out.alpha = vertexColor.a;
    
    // Calculate TBN matrix vectors
    vec3 N = normalize((transpose(inverse(modelMatrix)) * vec4(vertexNormal, 0.0)).xyz);
    vec3 T = normalize((transpose(inverse(modelMatrix)) * vec4(vertexTangent, 0.0)).xyz);
    vec3 B = cross(N, T);
    
    vs_out.normal = N;
    vs_out.tangent = T;
    vs_out.bitangent = B;
    
    vec4 worldPos = modelMatrix * vec4(vertexPos, 1.0);
    vs_out.fragPos = worldPos.xyz;

    gl_Position = projMatrix * viewMatrix * worldPos;
}
