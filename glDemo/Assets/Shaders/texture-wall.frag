#version 450 core

in SimplePacket {
	vec3 surfaceWorldPos;
	vec3 surfaceNormal;
	vec2 texCoord;
	mat3 TBN;
} inputFragment;

layout(location = 0) out vec4 fragColour;

// Directional light
uniform vec3 DIRDir;
uniform vec3 DIRCol;
uniform vec3 DIRAmb;

// Texture maps
layout(binding = 0) uniform sampler2D u_BaseColor;
layout(binding = 1) uniform sampler2D u_NormalMap;
layout(binding = 2) uniform sampler2D u_RoughnessMap;
layout(binding = 3) uniform sampler2D u_MetallicMap;
layout(binding = 4) uniform sampler2D u_HeightMap;

void main(void)
{
	vec3 albedo = texture(u_BaseColor, inputFragment.texCoord).rgb;

	// Normal mapping
	vec3 normalTex = texture(u_NormalMap, inputFragment.texCoord).rgb;
	normalTex = normalize(normalTex * 2.0 - 1.0);
	vec3 N = normalize(inputFragment.TBN * normalTex);

	// Light direction (already normalized)
	float lambert = max(dot(N, DIRDir), 0.0);
	vec3 diffuse = albedo * DIRCol * lambert;

	vec3 finalColor = DIRAmb + diffuse;
	fragColour = vec4(finalColor, 1.0);
}
