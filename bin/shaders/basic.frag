#version 410 core

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;

out vec4 fColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;


uniform vec3 directionalLightDirection[3];
uniform vec3 directionalLightColor[3];
uniform float directionalLightIntensity[3];
uniform float ambientStrength;
uniform float specularStrength;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

vec3 directionalAmbientTotal = vec3(0, 0, 0);
vec3 directionalDiffuseTotal = vec3(0, 0, 0);
vec3 directionalSpecularTotal = vec3(0, 0, 0);

mat3 normalMatrix;

void computeDirectionalLights()
{
    vec4 fPosEye = viewMatrix * modelMatrix * vec4(fPosition, 1.0f);
    vec3 normalEye = normalize(normalMatrix * fNormal);
    vec3 viewDir = normalize(- fPosEye.xyz);

    for (int i = 0;i < 3; i++)
    {
        vec3 lightDirN = vec3(normalize(viewMatrix * vec4(directionalLightDirection[i], 0.0f)));
        vec3 ambient = directionalLightIntensity[i] * ambientStrength * directionalLightColor[i];
        vec3 diffuse = directionalLightIntensity[i] * max(dot(normalEye, lightDirN), 0.0f) * directionalLightColor[i];
        vec3 reflectDir = reflect(-lightDirN, normalEye);
        float specCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
        vec3 specular = directionalLightIntensity[i] * specularStrength * specCoeff * directionalLightColor[i];

        directionalAmbientTotal += ambient;
        directionalDiffuseTotal += diffuse;
        directionalSpecularTotal += specular;
    }
}

void main() 
{
    normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
    computeDirectionalLights();

    vec3 color = min((directionalAmbientTotal + directionalDiffuseTotal) * texture(diffuseTexture, fTexCoords).rgb + directionalSpecularTotal * texture(specularTexture, fTexCoords).rgb, 1.0f);

    fColor = vec4(color, 1.0f);
}
