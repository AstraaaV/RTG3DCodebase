#version 450 core

// ── Inputs from the vertex shader ──────────────────────────────────────────
in SimplePacket {
    vec3 surfaceWorldPos;
    vec3 surfaceNormal;
    vec2 texCoord;
} fsIn;

// ── Outputs ────────────────────────────────────────────────────────────────
layout(location = 0) out vec4 fragColour;

// ── Scene uniforms ─────────────────────────────────────────────────────────
uniform sampler2D diffuseTex;   // bind texture unit 0
uniform vec3      viewPos;      // camera position in world space

// Point-light definition (keep identical to C++ struct)
struct PointLight {
    vec3  position;
    vec3  colour;
    vec3  ambient;
    float constant;
    float linear;
    float quadratic;
};

uniform int        numPointLights;         // how many are actually set (< 16)
uniform PointLight pointLights[16];        // hard cap = 16 (matches C++ loop)

// ── Material / shading params ──────────────────────────────────────────────
uniform float shininess = 32.0;   // override per-object if you like

// ── Helper: compute attenuation factor ─────────────────────────────────────
float computeAttenuation(PointLight light, float distance)
{
    return 1.0 / (light.constant +
                  light.linear * distance +
                  light.quadratic * distance * distance);
}

void main()
{
    // ── Base surface data ──────────────────────────────────────────────────
    vec3  albedo    = texture(diffuseTex, fsIn.texCoord).rgb;
    vec3  N         = normalize(fsIn.surfaceNormal);
    vec3  V         = normalize(viewPos - fsIn.surfaceWorldPos);

    vec3  result = vec3(0.0);

    // ── Loop over point lights ─────────────────────────────────────────────
    for (int i = 0; i < numPointLights; ++i)
    {
        PointLight Lgt = pointLights[i];

        vec3  Ldir     = Lgt.position - fsIn.surfaceWorldPos;
        float distance = length(Ldir);
        Ldir           = normalize(Ldir);

        // Diffuse (Lambert)
        float NdotL    = max(dot(N, Ldir), 0.0);
        vec3  diffuse  = Lgt.colour * NdotL;

        // Specular (Blinn-Phong)
        vec3  H        = normalize(Ldir + V);
        float NdotH    = max(dot(N, H), 0.0);
        float spec     = pow(NdotH, shininess);
        vec3  specular = Lgt.colour * spec;

        // Attenuation
        float atten    = computeAttenuation(Lgt, distance);

        // Ambient
        vec3 ambient   = Lgt.ambient;

        result += (ambient + (diffuse + specular) * atten);
    }

    fragColour = vec4(result * albedo, 1.0);
}
