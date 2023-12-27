#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=3) in vec4 modelMatrixCol0;
layout(location=4) in vec4 modelMatrixCol1;
layout(location=5) in vec4 modelMatrixCol2;
layout(location=6) in vec4 modelMatrixCol3;

uniform mat4 lightSpaceMatrix;

void main()
{
    mat4 modelMatrix;
    modelMatrix[0] = modelMatrixCol0;
    modelMatrix[1] = modelMatrixCol1;
    modelMatrix[2] = modelMatrixCol2;
    modelMatrix[3] = modelMatrixCol3;
    gl_Position = lightSpaceMatrix * modelMatrix * vec4(vPosition, 1.0f);
}