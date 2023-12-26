#version 410 core

out vec4 fColor;

in vec2 textureCoordinates;

uniform int i;
uniform int j;
uniform float blendFactor;
uniform sampler2D particleTexture[8];

void main()
{
    vec4 colorI = texture(particleTexture[i], textureCoordinates);
    vec4 colorJ = texture(particleTexture[j], textureCoordinates);
    fColor = mix(colorI, colorJ, blendFactor);
}