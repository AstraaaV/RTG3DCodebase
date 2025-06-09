#version 450 core

layout(location = 0) in vec3 aPos;
// Add other vertex attributes if needed (normals, texcoords, etc.)

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
