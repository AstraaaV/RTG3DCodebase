#version 450 core

in SimplePacket {
	vec3 colour;
	vec2 texCoord;
} inputFragment;

uniform sampler2D u_Diffuse; // Expecting tex like "ROCK"

layout (location = 0) out vec4 fragColour;

void main(void)
{
	vec4 texColor = texture(u_Diffuse, inputFragment.texCoord);

	// Use texture with vertex tint (optional)
	fragColour = texColor * vec4(inputFragment.colour, 1.0);
}
