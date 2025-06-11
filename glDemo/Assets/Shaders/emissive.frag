#version 450 core

in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D fireTexture;
uniform vec3 emissiveColor;
uniform int currentFrame;

const int cols = 8;
const int rows = 8;

void main()
{
    int xFrame = currentFrame % cols;
    int yFrame = currentFrame / cols;

    vec2 frameOffset = vec2(float(xFrame) / cols, float(yFrame) / rows);
    vec2 uvScale = vec2(1.0 / cols, 1.0 / rows);

    vec2 finalUV = frameOffset + vTexCoord * uvScale;
    vec4 texColor = texture(fireTexture, finalUV);

    FragColor = vec4(emissiveColor * texColor.rgb, texColor.a);
}
