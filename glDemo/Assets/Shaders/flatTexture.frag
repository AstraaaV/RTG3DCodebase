#version 450 core

// flat textured frag shader

uniform sampler2D texture;

in SimplePacket {

  vec3 texCoord;

} inputFragment;

layout (location=0) out vec4 fragColour;

void main(void) {

	fragColour = texture(tecture, inputFragment.texCoord);
}