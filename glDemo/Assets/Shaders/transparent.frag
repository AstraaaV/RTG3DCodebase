#version 450 core

in vec2 fragUV;
out vec4 fragColour;

uniform sampler2D u_Diffuse;

// Optional scrolling animation
uniform float time;

void main()
{
    vec2 uv = fragUV + vec2(time * 0.02, 0.0); // scrolls right

    vec4 tex = texture(u_Diffuse, uv);

    if (tex.a < 0.05)
        discard;

    float alphaFade = 0.35; // base transparency
    fragColour = vec4(tex.rgb, tex.a * alphaFade);
}
