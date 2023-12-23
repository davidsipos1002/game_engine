#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

uniform mat4 directionalLightSpaceMatrix[3];
out vec4 fPositionDirectionalLight[3];

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() 
{
	fPosition = vPosition;
	fNormal = vNormal;
	fTexCoords = vTexCoords;
	for (int i = 0; i < 3; i++) {
		fPositionDirectionalLight[i] = directionalLightSpaceMatrix[i] * modelMatrix * vec4(vPosition, 1.0f);
	}
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vPosition, 1.0f);
}
