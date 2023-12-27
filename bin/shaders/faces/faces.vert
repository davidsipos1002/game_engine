#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 gPosEye;
out vec3 gNormalEye;
out vec3 gLightDirection;

vec3 lightDirection = vec3(0, 1, 1);

void main()
{
    vec4 eyePos = viewMatrix * modelMatrix * vec4(vPosition, 1.0f);
    gPosEye = eyePos.xyz;
    mat3 normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
    gNormalEye = normalize(normalMatrix * vNormal);
    gLightDirection = vec3(normalize(viewMatrix * vec4(lightDirection, 0.0f)));
    gl_Position = projectionMatrix * eyePos;
}