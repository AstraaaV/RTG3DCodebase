#version 450 core
layout(location = 0) in vec3 vertexPos;
layout(location = 2) in vec2 vertexTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec2 fragUV;

void main()
{
    fragUV = vertexTexCoord;
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(vertexPos, 1.0);
}
