#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

uniform mat4 directionalLightSpaceMatrix[3];
out vec4 fPositionDirectionalLight[3];

uniform mat4 spotLightSpaceMatrix[5];
out vec4 fPositionSpotLight[5];

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoords;
flat out vec2 fLightData;
flat out mat4 fModelMatrix;

uniform float ambientStrength;
uniform float specularStrength;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() 
{
	fPosition = vPosition;
	fNormal = vNormal;
	fTexCoords = vTexCoords;
	fLightData = vec2(ambientStrength, specularStrength);
	fModelMatrix = modelMatrix;
	for (int i = 0; i < 3; i++) 
	{
		fPositionDirectionalLight[i] = directionalLightSpaceMatrix[i] * modelMatrix * vec4(vPosition, 1.0f);
	}
	for (int i = 0;i < 5; i++) 
	{
		fPositionSpotLight[i] = spotLightSpaceMatrix[i] * modelMatrix * vec4(vPosition, 1.0f);
	}
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vPosition, 1.0f);
}
