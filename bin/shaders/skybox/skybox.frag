#version 410 core

in vec3 textureCoordinates;
out vec4 color;

uniform vec3 colorModifier;
uniform samplerCube skybox;

void main()
{
    color = vec4(colorModifier, 1.0f) * texture(skybox, textureCoordinates);
}
