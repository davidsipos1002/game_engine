#version 410 core

out vec4 fColor;

in vec2 textureCoordinates;
in vec2 fIndex;
in float fBlendFactor;
in vec4 fPosEye;

uniform float fogDensity;
uniform vec3 fogColor;
uniform sampler2D particleTexture[8];

float computeFog()
{
    float dist = length(fPosEye.xyz);
    float fogFactor = exp(-pow(dist * fogDensity, 2));
    return clamp(fogFactor, 0.0f, 1.0f); 
}

void main()
{
    vec4 colorI = texture(particleTexture[int(fIndex.x)], textureCoordinates);
    vec4 colorJ = texture(particleTexture[int(fIndex.y)], textureCoordinates);
    vec4 color = mix(colorI, colorJ, fBlendFactor);
    float fogFactor = computeFog();
    fColor = mix(vec4(fogColor, color.a), color, fogFactor);
}