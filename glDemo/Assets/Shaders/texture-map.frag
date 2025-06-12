#version 450 core

in vec2 fragUV;
in vec3 fragNormal;
in vec3 fragWorldPos;

uniform sampler2D u_Diffuse;

layout (location = 0) out vec4 fragColour;

void main()
{
    vec3 baseColor = texture(u_Diffuse, fragUV).rgb;

    // Mix base color with white
    float tintStrength = 0.75;
    vec3 whiteTint = mix(baseColor, vec3(1.0), tintStrength);

    fragColour = vec4(whiteTint, 1.0);
}
