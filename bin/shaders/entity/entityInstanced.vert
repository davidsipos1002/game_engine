#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;
layout(location=3) in vec4 modelMatrixCol0;
layout(location=4) in vec4 modelMatrixCol1;
layout(location=5) in vec4 modelMatrixCol2;
layout(location=6) in vec4 modelMatrixCol3;
layout(location=7) in vec2 lightData;

uniform mat4 directionalLightSpaceMatrix[3];
out vec4 fPositionDirectionalLight[3];

uniform mat4 spotLightSpaceMatrix[5];
out vec4 fPositionSpotLight[5];

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoords;
flat out vec2 fLightData;
flat out mat4 fModelMatrix;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() 
{
	fPosition = vPosition;
	fNormal = vNormal;
	fTexCoords = vTexCoords;
	fLightData = lightData;
	fModelMatrix[0] = modelMatrixCol0;
	fModelMatrix[1] = modelMatrixCol1;
	fModelMatrix[2] = modelMatrixCol2;
	fModelMatrix[3] = modelMatrixCol3;
	for (int i = 0; i < 3; i++) 
	{
		fPositionDirectionalLight[i] = directionalLightSpaceMatrix[i] * fModelMatrix * vec4(vPosition, 1.0f);
	}
	for (int i = 0;i < 5; i++) 
	{
		fPositionSpotLight[i] = spotLightSpaceMatrix[i] * fModelMatrix * vec4(vPosition, 1.0f);
	}
	gl_Position = projectionMatrix * viewMatrix * fModelMatrix * vec4(vPosition, 1.0f);
}
