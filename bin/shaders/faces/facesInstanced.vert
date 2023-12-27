#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=3) in vec4 modelMatrixCol0;
layout(location=4) in vec4 modelMatrixCol1;
layout(location=5) in vec4 modelMatrixCol2;
layout(location=6) in vec4 modelMatrixCol3;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 gPosEye;
out vec3 gNormalEye;
out vec3 gLightDirection;

vec3 lightDirection = vec3(0, 1, 1);

void main()
{
    mat4 modelMatrix;
    modelMatrix[0] = modelMatrixCol0;
    modelMatrix[1] = modelMatrixCol1;
    modelMatrix[2] = modelMatrixCol2;
    modelMatrix[3] = modelMatrixCol3;
    vec4 eyePos = viewMatrix * modelMatrix * vec4(vPosition, 1.0f);
    gPosEye = eyePos.xyz;
    mat3 normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
    gNormalEye = normalize(normalMatrix * vNormal);
    gLightDirection = vec3(normalize(viewMatrix * vec4(lightDirection, 0.0f)));
    gl_Position = projectionMatrix * eyePos;
}