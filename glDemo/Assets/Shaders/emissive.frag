#version 450 core

in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D textureSampler;
uniform int frame;

void main()
{
    // Sprite sheet layout (8x8)
    int row = frame / 8;
    int col = frame % 8;

    vec2 spriteSize = vec2(1.0 / 8.0);
    vec2 spriteUV = texCoord * spriteSize + vec2(col, row) * spriteSize;

    vec4 surfaceColor = texture(textureSampler, spriteUV);
    fragColor = surfaceColor;
}
