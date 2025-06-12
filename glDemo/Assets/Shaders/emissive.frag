#version 450 core

in vec2 fragUV;
out vec4 fragColour;

uniform sampler2D u_Diffuse;

void main()
{
    vec3 baseColor = texture(u_Diffuse, fragUV).rgb;

    // Add glow by boosting brightness
    vec3 glowColor = baseColor * vec3(2.5);

    fragColour = vec4(glowColor, 1.0);
}
