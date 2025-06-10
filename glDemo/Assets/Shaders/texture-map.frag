#version 450 core

in vec2 texCoord;
uniform sampler2D u_Diffuse;

out vec4 fragColor;

void main()
{
    fragColor = texture(u_Diffuse, texCoord);
}