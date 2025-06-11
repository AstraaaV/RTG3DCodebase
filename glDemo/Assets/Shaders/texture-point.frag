#version 450 core

uniform vec3 Torch1pos;
uniform vec3 Torch1col;
uniform vec3 Torch1amb;
uniform vec3 Torch1atten;

uniform vec3 viewPos;
uniform sampler2D tex;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Ambient
    vec3 ambient = Torch1amb * texture(tex, TexCoord).rgb;

    // Diffuse
    vec3 lightDir = normalize(Torch1pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * Torch1col * texture(tex, TexCoord).rgb;

    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * Torch1col;

    // Attenuation
    float distance = length(Torch1pos - FragPos);
    float attenuation = 1.0 / (Torch1atten.x + Torch1atten.y * distance + Torch1atten.z * distance * distance);

    vec3 result = (ambient + diffuse + specular) * attenuation;
    FragColor = vec4(result, 1.0);
}
