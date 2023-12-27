#version 410 core

in vec4 fPosition;

uniform vec3 lightPosition;

float farPlane = 20.0f;

void main ()
{
    float lightDistance = length(fPosition.xyz - lightPosition);
    lightDistance /= farPlane;
    gl_FragDepth = lightDistance;
}