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

uniform vec3 pointLightPosition[10];
uniform vec3 pointLightColor[10];
uniform float pointLightIntensity[10];

uniform float ambientStrength;
uniform float specularStrength;


uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

vec3 directionalAmbientTotal = vec3(0, 0, 0);
vec3 directionalDiffuseTotal = vec3(0, 0, 0);
vec3 directionalSpecularTotal = vec3(0, 0, 0);

vec3 pointAmbientTotal = vec3(0, 0, 0);
vec3 pointDiffuseTotal = vec3(0, 0, 0);
vec3 pointSpecularTotal = vec3(0, 0, 0);

mat3 normalMatrix;
vec4 fPosEye;
vec3 normalEye;
vec3 viewDir;

float constant = 1.0f;
float linear = 0.0045f;
float quadratic = 0.0075f;

void computeDirectionalLights()
{
    for (int i = 0; i < 3; i++)
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

void computePointLights()
{
    for (int i = 0; i < 10; i++)
    {
        vec3 lightPosEye = vec3(viewMatrix * vec4(pointLightPosition[i], 0.0f));
        vec3 lightVector = lightPosEye - fPosEye.xyz;
        float dist = length(lightVector);
        lightVector = normalize(lightVector);
        float brightness = max(dot(normalEye, lightVector), 0.0f);
        float attenuation = 1.0f / (constant + linear * dist + quadratic * (dist * dist));
        vec3 ambient = pointLightIntensity[i] * attenuation * ambientStrength * pointLightColor[i];
        vec3 diffuse = pointLightIntensity[i] * attenuation * max(dot(normalEye, lightVector), 0.0f) * pointLightColor[i];
        vec3 reflectDir = reflect(-lightVector, normalEye);
        float specCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
        vec3 specular = pointLightIntensity[i] * attenuation * specularStrength * specCoeff * pointLightColor[i]; 

        pointAmbientTotal += ambient;
        pointDiffuseTotal += diffuse;
        pointSpecularTotal += specular;
    }
}

void main() 
{
    normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
    fPosEye = viewMatrix * modelMatrix * vec4(fPosition, 1.0f);
    normalEye = normalize(normalMatrix * fNormal);
    viewDir = normalize(- fPosEye.xyz);
    computeDirectionalLights();
    computePointLights();

    vec3 diffuseTextureColor = texture(diffuseTexture, fTexCoords).rgb;
    vec3 specularTextureColor = texture(specularTexture, fTexCoords).rgb;

    vec3 finalDirectional = min((directionalAmbientTotal + directionalDiffuseTotal) * 
                     diffuseTextureColor + directionalSpecularTotal * specularTextureColor, 1.0f);

    vec3 finalPoint = min((pointAmbientTotal + pointDiffuseTotal) * 
                     diffuseTextureColor + pointSpecularTotal * specularTextureColor, 1.0f);

    vec3 color = finalDirectional + finalPoint;
    fColor = vec4(color, 1.0f);
}
