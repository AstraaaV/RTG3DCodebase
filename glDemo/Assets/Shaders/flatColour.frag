#version 450 core

// flat colour from vertex colours


in SimplePacket {

  vec3 colour;

} inputFragment;


layout (location=0) out vec4 fragColour;

void main(void) {

	fragColour = vec4(1.0, 1.0, 1.0, 1.0);
}
