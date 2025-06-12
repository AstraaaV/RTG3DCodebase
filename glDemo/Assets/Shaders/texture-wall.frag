#version 450 core

in SimplePacket {
    vec3 surfaceWorldPos;
    vec3 surfaceNormal;
    vec2 texCoord;
    mat3 TBN;
} inputFragment;

out vec4 FragColor;

// Texture samplers
uniform sampler2D u_BaseColor;
uniform sampler2D u_NormalMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_MetallicMap;

// Directional Light
uniform vec3 DIRDir;
uniform vec3 DIRCol;
uniform vec3 DIRAmb;

// View position
uniform vec3 viewPos;

void main()
{
    // Sample textures
    vec3 albedo = texture(u_BaseColor, inputFragment.texCoord).rgb;

    // Apply subtle warm tint
    vec3 yellowTint = vec3(1.03, 1.025, 0.99);
    albedo *= yellowTint;
    albedo = max(albedo, vec3(0.2));

    float roughness = texture(u_RoughnessMap, inputFragment.texCoord).r;
    float metallic = texture(u_MetallicMap, inputFragment.texCoord).r;

    // Normal mapping
    vec3 sampledNormal = texture(u_NormalMap, inputFragment.texCoord).rgb;
    sampledNormal = normalize(sampledNormal * 2.0 - 1.0);
    vec3 N = normalize(inputFragment.TBN * sampledNormal);

    vec3 V = normalize(viewPos - inputFragment.surfaceWorldPos);
    vec3 L = normalize(-DIRDir);
    vec3 H = normalize(V + L);

    // Diffuse
    float NdotL = max(dot(N, L), 0.0);
    vec3 diffuse = albedo * DIRCol * NdotL;

    // Specular
    float NdotH = max(dot(N, H), 0.0);
    float specularStrength = pow(NdotH, 32.0 * (1.0 - roughness));
    vec3 specular = specularStrength * DIRCol * mix(vec3(0.04), albedo, metallic);

    // Final lighting result
    vec3 result = (DIRAmb * albedo) + diffuse + specular;

    // Slight brightness boost
    result *= 1.50;

    FragColor = vec4(result, 1.0);
}
