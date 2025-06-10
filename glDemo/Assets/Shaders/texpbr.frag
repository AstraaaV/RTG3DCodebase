#version 450 core

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    vec3 ambient;
};

layout(binding = 0) uniform sampler2D u_BaseColor;
layout(binding = 1) uniform sampler2D u_NormalMap;
layout(binding = 2) uniform sampler2D u_RoughnessMap;
layout(binding = 3) uniform sampler2D u_MetallicMap;
layout(binding = 4) uniform sampler2D u_HeightMap;
layout(binding = 5) uniform sampler2D u_EmissiveMap;

uniform DirectionalLight dirLight;
uniform vec3 viewPos;
uniform float time;

in VS_OUT {
    vec3 fragPos;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
} fs_in;

layout(location = 0) out vec4 fragColour;

// Parallax mapping function (simple)
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    float height = texture(u_HeightMap, texCoords).r;
    float heightScale = 0.05;  // tweak this value for depth effect
    vec2 p = viewDir.xy / viewDir.z * (height * heightScale);
    return texCoords - p;
}

void main()
{
    // Build TBN matrix
    mat3 TBN = mat3(normalize(fs_in.tangent), normalize(fs_in.bitangent), normalize(fs_in.normal));

    // View direction in tangent space
    vec3 viewDir = normalize(TBN * normalize(viewPos - fs_in.fragPos));

    // Parallax offset texture coords
    vec2 texCoords = ParallaxMapping(fs_in.texCoord, viewDir);

    // Sample textures
    vec3 baseColor = texture(u_BaseColor, texCoords);
    vec3 normalMap = texture(u_NormalMap, texCoords).rgb;
    float roughness = texture(u_RoughnessMap, texCoords).r;
    float metallic = texture(u_MetallicMap, texCoords).r;
    vec3 emissiveCol = texture(u_EmissiveMap, texCoords).rgb;

    if (baseColor.a < 0.1)
        discard;

    float flicker = sin(time * 10.0) * 0.2 + 0.8;
    emissiveCol *= flicker;

    // Transform normal map value from [0,1] to [-1,1]
    vec3 N = normalize(normalMap * 2.0 - 1.0);
    N = normalize(TBN * N);

    // Simple Blinn-Phong directional light
    vec3 L = normalize(-dirLight.direction);
    vec3 V = normalize(viewPos - fs_in.fragPos);
    vec3 H = normalize(L + V);

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotH = max(dot(N, H), 0.0);

    // Ambient
    vec3 ambient = dirLight.ambient * baseColor.rgb;

    // Diffuse
    vec3 diffuse = baseColor.rgb * dirLight.color * NdotL;

    // Specular (using roughness & metallic as rough approximation)
    float specularPower = mix(256.0, 16.0, roughness); // roughness controls shininess
    vec3 specularColor = mix(vec3(0.04), baseColor.rgb, metallic); // metallic controls specular color
    float spec = pow(NdotH, specularPower);
    vec3 specular = specularColor * dirLight.color * spec;

    vec3 color = ambient + diffuse + specular + emissiveCol;

    fragColour = vec4(color, baseColor.a);
}
