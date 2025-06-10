#version 450 core

in SimplePacket {
    vec3 surfaceWorldPos;
    vec3 surfaceNormal;
    vec2 texCoord;
    mat3 TBN;
} inputFragment;

out vec4 FragColor;

// PBR Textures
uniform sampler2D u_BaseColor;
uniform sampler2D u_NormalMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_MetallicMap;

// Lighting
uniform vec3 viewPos;

struct PointLight {
    vec3 position;
    vec3 colour;
    float intensity;
};

#define MAX_POINT_LIGHTS 16
uniform int numPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

void main()
{
    vec3 albedo = texture(u_BaseColor, inputFragment.texCoord).rgb;
    float roughness = texture(u_RoughnessMap, inputFragment.texCoord).r;
    float metallic = texture(u_MetallicMap, inputFragment.texCoord).r;

    // Sample and apply normal map
    vec3 sampledNormal = texture(u_NormalMap, inputFragment.texCoord).rgb;
    sampledNormal = normalize(sampledNormal * 2.0 - 1.0);
    vec3 N = normalize(inputFragment.TBN * sampledNormal);

    vec3 V = normalize(viewPos - inputFragment.surfaceWorldPos);
    vec3 resultColor = vec3(0.0);

    for (int i = 0; i < numPointLights; ++i)
    {
        vec3 L = normalize(pointLights[i].position - inputFragment.surfaceWorldPos);
        vec3 H = normalize(V + L);

        float distance = length(pointLights[i].position - inputFragment.surfaceWorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 lightColor = pointLights[i].colour * pointLights[i].intensity * attenuation;

        // Diffuse
        float NdotL = max(dot(N, L), 0.0);
        vec3 diffuse = albedo * lightColor * NdotL;

        // Specular (simplified Blinn-Phong)
        float NdotH = max(dot(N, H), 0.0);
        float specularStrength = pow(NdotH, 32.0 * (1.0 - roughness));
        vec3 specular = specularStrength * lightColor * mix(vec3(0.04), albedo, metallic);

        resultColor += diffuse + specular;
    }

    FragColor = vec4(resultColor, 1.0);
}
