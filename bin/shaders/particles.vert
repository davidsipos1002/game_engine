#version 410 core

layout(location=0) in vec2 vPosition;

out vec2 textureCoordinates;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    textureCoordinates = vPosition + vec2(0.5f, 0.5f);
    textureCoordinates.y = 1.0f - textureCoordinates.y;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vPosition, 0.0, 1.0f);
}