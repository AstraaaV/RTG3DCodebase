#version 450 core

layout(location = 0) in vec2 TexCoord;
layout(location = 1) in vec3 FragPos;
layout(location = 2) in vec3 Normal;

layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D u_DiffuseMap;
layout(binding = 1) uniform sampler2D u_NormalMap;
layout(binding = 2) uniform sampler2D u_RoughnessMap;

uniform vec3 lightPos = vec3(5.0, 10.0, 5.0);
uniform vec3 viewPos = vec3(0.0, 5.0, 10.0);

void main()
{
    vec3 normalMap = texture(u_NormalMap, TexCoord).rgb;
    vec3 norm = normalize(normalMap * 2.0 - 1.0); // unpack

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0 * (1.0 - texture(u_RoughnessMap, TexCoord).r));

    vec3 baseColor = texture(u_DiffuseMap, TexCoord).rgb;
    vec3 lighting = baseColor * diff + vec3(spec);

    FragColor = vec4(lighting, 1.0);
}
