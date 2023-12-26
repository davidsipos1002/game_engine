#version 410 core

out vec4 fColor;

in vec2 textureCoordinates;
in vec2 fIndex;
in float fBlendFactor;

uniform sampler2D particleTexture[8];

void main()
{
    vec4 colorI = texture(particleTexture[int(fIndex.x)], textureCoordinates);
    vec4 colorJ = texture(particleTexture[int(fIndex.y)], textureCoordinates);
    fColor = mix(colorI, colorJ, fBlendFactor);
}