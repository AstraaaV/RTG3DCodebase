#version 450 core

out vec4 FragColor;

uniform vec4 ghostColor; // RGBA color with alpha for transparency

void main()
{
    FragColor = ghostColor;
}
