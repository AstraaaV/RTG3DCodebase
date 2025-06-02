#version 450 core

// Diffuse texture - directional light

// Texture sampler (for diffuse surface colour)
layout(binding = 0) uniform sampler2D texture;

// Directional light model
uniform vec3 DIRDir;
uniform vec3 DIRCol;
uniform vec3 DIRAmb;


in SimplePacket {
	
	vec3 surfaceWorldPos;
	vec3 surfaceNormal;
	vec2 texCoord;

} inputFragment;


layout (location=0) out vec4 fragColour;

void main(void) {

	// calculate lambertian (l)
	vec3 N = normalize(inputFragment.surfaceNormal);
	float l = dot(N, DIRDir);

	// Calculate diffuse brightness / colour for fragment
	vec4 surfaceColour = texture2D(texture, inputFragment.texCoord);
	vec3 diffuseColour = surfaceColour.rgb * DIRCol * l;

	fragColour = vec4(DIRAmb,1.0)+vec4(diffuseColour, 1.0);
	//fragColour = vec4(vec3(l, l, l), 1.0);
}