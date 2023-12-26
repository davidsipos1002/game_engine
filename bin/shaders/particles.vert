#version 410 core

layout(location=0) in vec2 vPosition;
layout(location=1) in vec4 col0;
layout(location=2) in vec4 col1;
layout(location=3) in vec4 col2;
layout(location=4) in vec4 col3;
layout(location=5) in int i;
layout(location=6) in int j;
layout(location=7) in float blendFactor;

out vec2 textureCoordinates;
out vec2 fIndex;
out float fBlendFactor;
out vec4 fPosEye;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
mat4 modelMatrix;

void main()
{
    modelMatrix[0] = col0;
    modelMatrix[1] = col1;
    modelMatrix[2] = col2;
    modelMatrix[3] = col3;
    fIndex.x = i;
    fIndex.y = j;
    fBlendFactor = blendFactor;
    textureCoordinates = vPosition + vec2(0.5f, 0.5f);
    textureCoordinates.y = 1.0f - textureCoordinates.y;
    fPosEye = viewMatrix * modelMatrix * vec4(vPosition, 0.0f, 1.0f);
    gl_Position = projectionMatrix * fPosEye;
}