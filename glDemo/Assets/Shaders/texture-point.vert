#version 450 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
    // Transform vertex position to world space
    FragPos = vec3(model * vec4(inPos, 1.0));
    
    // Correctly transform normal
    Normal = mat3(transpose(inverse(model))) * inNormal;

    // Pass texture coordinate to fragment shader
    TexCoord = inTexCoord;

    // Calculate final screen position
    gl_Position = proj * view * vec4(FragPos, 1.0);
}
