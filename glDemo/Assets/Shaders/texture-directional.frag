#version 450 core

// Diffuse texture - directional light

// Texture sampler (for diffuse surface colour)
layout(binding = 0) uniform sampler2D texture;

// Directional light model
uniform vec3 SUNDir;
uniform vec3 SUNCol;
uniform vec3 SUNAmb;
uniform bool lightsEnabled;

in SimplePacket {
	
	vec3 surfaceWorldPos;
	vec3 surfaceNormal;
	vec2 texCoord;

} inputFragment;


layout (location=0) out vec4 fragColour;

void main(void) {

	vec4 surfaceColour = texture(texture, inputFragment.texCoord);

	if(!lightsEnabled)
	{
		fragColour = surfaceColour;
		return;
	}

	// calculate lambertian (l)
	vec3 N = normalize(inputFragment.surfaceNormal);
	float l = max(dot(N, SUNDir), 0.0);

	// Calculate diffuse brightness / colour for fragment
	vec3 diffuseColour = surfaceColour.rgb * SUNCol * l;

	fragColour = vec4(SUNAmb + diffuseColour, 1.0);
	//fragColour = vec4(vec3(l, l, l), 1.0);
}
