#version 410 core

in vec2 fTexCoords;

out vec4 fColor;

uniform sampler2D shadowMap;

void main() 
{    
    fColor = vec4(vec3(texture(shadowMap, fTexCoords).r), 1.0f);
}