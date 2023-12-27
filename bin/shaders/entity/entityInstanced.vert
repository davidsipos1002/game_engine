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

out vec3 fPosWorld;
out vec3 fPosEye; 
out vec3 fNormalEye;
out vec2 fTexCoords;
flat out vec2 fLightData;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() 
{
	fTexCoords = vTexCoords;
	fLightData = lightData;
	mat4 modelMatrix;
	modelMatrix[0] = modelMatrixCol0;
	modelMatrix[1] = modelMatrixCol1;
	modelMatrix[2] = modelMatrixCol2;
	modelMatrix[3] = modelMatrixCol3;
	vec4 worldPos = modelMatrix * vec4(vPosition, 1.0f);
	vec4 eyePos = viewMatrix * worldPos;
	fPosWorld = worldPos.xyz;
	fPosEye = eyePos.xyz;
    mat3 normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
    fNormalEye = normalize(normalMatrix * vNormal);
	for (int i = 0; i < 3; i++) 
	{
		fPositionDirectionalLight[i] = directionalLightSpaceMatrix[i] * worldPos;
	}
	for (int i = 0;i < 5; i++) 
	{
		fPositionSpotLight[i] = spotLightSpaceMatrix[i] * worldPos;
	}
	gl_Position = projectionMatrix * eyePos;
}
