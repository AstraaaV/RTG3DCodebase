#version 450 core

in SimplePacket {
    vec3 colour;
    vec2 texCoord;
} inputFragment;

uniform sampler2D u_Diffuse;

layout (location = 0) out vec4 fragColour;

void main(void) {
    vec3 texColor = texture(u_Diffuse, inputFragment.texCoord).rgb;
    fragColour = vec4(texColor, 1.0); // You can multiply by colour if needed
}
