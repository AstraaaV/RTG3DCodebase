#version 450 core

// flat textured frag shader

uniform sampler2D texture;

in SimplePacket {

  vec2 texCoord;

} inputFragment;

layout (location=0) out vec4 fragColour;

void main(void) {

	vec4 texColor = texture(texture, inputFragment.texCoord);

	if(texColor.a < 0.1)
		discard;

	fragColour = texColor;
}