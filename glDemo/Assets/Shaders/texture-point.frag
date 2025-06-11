#version 450 core

#define MAX_POINT_LIGHTS 16

struct PointLight {
    vec3 position;
    vec3 color;
    vec3 ambient;

    float constant;
    float linear;
    float quadratic;
};

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numPointLights;
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
    vec3 result = vec3(0.0);

    for (int i = 0; i < numPointLights; ++i)
    {
        // Ambient
        vec3 ambient = pointLights[i].ambient * texture(tex, TexCoord).rgb;

        // Diffuse
        vec3 lightDir = normalize(pointLights[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * pointLights[i].color * texture(tex, TexCoord).rgb;

        // Specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = spec * pointLights[i].color;

        // Attenuation
        float distance = length(pointLights[i].position - FragPos);
        float attenuation = 1.0 / (pointLights[i].constant +
                                   pointLights[i].linear * distance +
                                   pointLights[i].quadratic * (distance * distance));

        result += (ambient + diffuse + specular) * attenuation;
    }

    FragColor = vec4(result, 1.0);
}
