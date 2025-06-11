#version 450 core

in vec2 fragUV;
out vec4 fragColour;

uniform sampler2D u_Diffuse;
uniform float u_Time; // Passed in from your program every frame

void main(void) {
    // Scroll UV coordinates to simulate motion
    vec2 scrolledUV = fragUV + vec2(u_Time * 0.02, u_Time * 0.01); 

    // Sample texture with alpha
    vec4 texColor = texture(u_Diffuse, scrolledUV);

    // Discard fully transparent fragments (optional)
    if (texColor.a < 0.05)
        discard;

    // Blend texture color with some subtle tint
    fragColour = vec4(texColor.rgb * 0.9, texColor.a * 0.5);
}
