#version 450 core

in SimplePacket {
    vec3 surfaceWorldPos;
    vec3 surfaceNormal;
    vec2 texCoord;
    mat3 TBN;
} inputFragment;

out vec4 FragColor;

// Textures
uniform sampler2D u_BaseColor;
uniform sampler2D u_NormalMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_MetallicMap;

// Directional Light
uniform vec3 DIRDir;
uniform vec3 DIRCol;
uniform vec3 DIRAmb;

// Camera position
uniform vec3 viewPos;

void main()
{
    vec3 albedo = texture(u_BaseColor, inputFragment.texCoord).rgb;
    float roughness = texture(u_RoughnessMap, inputFragment.texCoord).r;
    float metallic = texture(u_MetallicMap, inputFragment.texCoord).r;

    // Apply normal map in tangent space
    vec3 sampledNormal = texture(u_NormalMap, inputFragment.texCoord).rgb;
    sampledNormal = normalize(sampledNormal * 2.0 - 1.0);
    vec3 N = normalize(inputFragment.TBN * sampledNormal);

    vec3 V = normalize(viewPos - inputFragment.surfaceWorldPos);
    vec3 L = normalize(-DIRDir); // Directional light vector
    vec3 H = normalize(V + L);

    // Light color
    vec3 lightColor = DIRCol;

    // Diffuse
    float NdotL = max(dot(N, L), 0.0);
    vec3 diffuse = albedo * lightColor * NdotL;

    // Specular (Blinn-Phong approx)
    float NdotH = max(dot(N, H), 0.0);
    float specularStrength = pow(NdotH, 32.0 * (1.0 - roughness));
    vec3 specular = specularStrength * lightColor * mix(vec3(0.04), albedo, metallic);

    vec3 resultColor = DIRAmb * albedo + diffuse + specular;
    FragColor = vec4(resultColor, 1.0);
}
