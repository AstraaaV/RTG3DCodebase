#version 450 core

// Uniforms for point light
uniform vec3 pointPos; // Light world position
uniform vec3 pointCol; // Diffuse light colour
uniform vec3 ambientCol; // Ambient colour

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
	
	// Direction from surface to light
	vec3 lightDir = normalize(pointPos - inputFragment.surfaceWorldPos);

	// Distance to light
	float distance = length(pointPos - inputFragment.surfaceWorldPos);

	// Attentuation based on distance
	float att = 1.0 / (1.0 + 0.22 * distance + 0.20 * distance * distance);

	// Lambertian
	float diff = max(dot(N, lightDir), 0.0);

	// Get surface texture colour
	vec4 surfaceColour = texture(texture, inputFragment.texCoord);
	vec3 diffuseColour = surfaceColour.rgb * pointCol * diff;

	// Combine ambient and diffuse with att
	vec3 finalColour = (ambientCol + diffuseColour) * att;

	// Set fragment colour
	fragColour = vec4(finalColour, surfaceColour.a);
}
