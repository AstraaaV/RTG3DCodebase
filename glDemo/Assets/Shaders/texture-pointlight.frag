#version 450 core

layout(binding = 0) uniform sampler2D texture;

uniform vec3 POINTPos;     // Position of the point light
uniform vec3 POINTCol;     // Colour of the point light
uniform vec3 POINTAmb;     // Ambient colour
uniform float POINTConst;  // Constant attenuation
uniform float POINTLinear; // Linear attenuation
uniform float POINTQuad;   // Quadratic attenuation

in SimplePacket {
    vec3 surfaceWorldPos;
    vec3 surfaceNormal;
    vec2 texCoord;
} inputFragment;

layout(location = 0) out vec4 fragColour;

void main(void) {
    vec3 N = normalize(inputFragment.surfaceNormal);
    vec3 L = normalize(POINTPos - inputFragment.surfaceWorldPos);

    // Lambertian reflectance
    float lambertian = max(dot(N, L), 0.0);

    // Distance to light
    float distance = length(POINTPos - inputFragment.surfaceWorldPos);

    // Attenuation
    float attenuation = 1.0 / (POINTConst + POINTLinear * distance + POINTQuad * distance * distance);

    // Surface texture
    vec4 surfaceColour = texture(texture, inputFragment.texCoord);

    // Lighting equation
    vec3 ambient = POINTAmb * surfaceColour.rgb;
    vec3 diffuse = lambertian * POINTCol * surfaceColour.rgb * attenuation;

    fragColour = vec4(ambient + diffuse, 1.0);
}
