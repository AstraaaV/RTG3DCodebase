#version 450 core

// Uniforms for point light
uniform int numPointLights;
uniform vec3 pointPos[16]; // Light world position
uniform vec3 pointCol[16]; // Diffuse light colour
uniform vec3 ambientCol[16]; // Ambient colour

// Inputs from vertex shader
in SimplePacket {
	
	vec3 surfaceWorldPos;
	vec3 surfaceNormal;
	vec2 texCoord;

} inputFragment;

// Texture
uniform sampler2D texture;

// Output colour
layout (location=0) out vec4 fragColour;

void main(void) {
	// Normalize surface normal
	vec3 N = normalize(inputFragment.surfaceNormal);
	vec4 surfaceColour = texture(texture, inputFragment.texCoord);

	vec3 finalColour = vec3(0.0);

	for (int i = 0; i < numPointLights; ++i)
	{
		// Direction from surface to light
		vec3 lightDir = normalize(pointPos[i] - inputFragment.surfaceWorldPos);

		// Distance to light
		float distance = length(pointPos[i] - inputFragment.surfaceWorldPos);

		// Attentuation based on distance
		float att = 1.0 / (1.0 + 0.22 * distance + 0.20 * distance * distance);

		// Lambertian
		float diff = max(dot(N, lightDir), 0.0);

		vec3 diffuseColour = surfaceColour.rgb * pointCol[i] * diff;
		vec3 ambient = ambientCol[i] * surfaceColour.rgb;

		finalColour += (ambient + diffuse) * att;
	}


	// Set fragment colour
	fragColour = vec4(finalColour, surfaceColour.a);
}
