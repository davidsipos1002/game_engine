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
uniform int directionalLightIsShadowCasting[3];
uniform sampler2D directionalLightShadowMap[3];
in vec4 fPositionDirectionalLight[3];

uniform vec3 pointLightPosition[10];
uniform vec3 pointLightColor[10];
uniform float pointLightIntensity[10];

uniform vec3 spotLightPosition[10];
uniform vec3 spotLightDirection[10];
uniform vec3 spotLightColor[10];
uniform float spotLightCutoff[10];
uniform float spotLightIntensity[10];
uniform int spotLightIsShadowCasting[10];
uniform sampler2D spotLightShadowMap[10];
in vec4 fPositionSpotLight[10];

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

vec3 spotAmbientTotal = vec3(0, 0, 0);
vec3 spotDiffuseTotal = vec3(0, 0, 0);
vec3 spotSpecularTotal = vec3(0, 0, 0);

mat3 normalMatrix;
vec4 fPosEye;
vec3 normalEye;
vec3 viewDir;

float constant = 1.0f;
float linear = 0.0045f;
float quadratic = 0.0075f;

float computeDirectionalAndSpotLightShadow(bool directional, int i) {
    vec3 normalizedCoords;
    if (directional) {
        normalizedCoords = fPositionDirectionalLight[i].xyz / fPositionDirectionalLight[i].w;
    } else {
        normalizedCoords = fPositionSpotLight[i].xyz / fPositionSpotLight[i].w;
    }
    normalizedCoords = normalizedCoords * 0.5 + 0.5;
    if (normalizedCoords.z > 1.0f)
        return 0.0f;
    float currentDepth = normalizedCoords.z;
    float bias = 0.005f;
    float shadow = 0.0;
    vec2 texelSize;
    if (directional) {
        texelSize = 1.0 / textureSize(directionalLightShadowMap[i], 0);
    } else {
        texelSize = 1.0 / textureSize(spotLightShadowMap[i], 0);
    }
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth;
            if (directional) {
                pcfDepth = texture(directionalLightShadowMap[i], normalizedCoords.xy + vec2(x, y) * texelSize).r; 
            } else {
                pcfDepth = texture(spotLightShadowMap[i], normalizedCoords.xy + vec2(x, y) * texelSize).r; 
            }
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }   
    shadow /= 9.0;
    return shadow;
}

void computeDirectionalLights()
{
    for (int i = 0; i < 3; i++)
    {
        if (directionalLightIntensity[i] > 0.0f) {
            vec3 lightDirN = vec3(normalize(viewMatrix * vec4(directionalLightDirection[i], 0.0f)));
            vec3 ambient = directionalLightIntensity[i] * ambientStrength * directionalLightColor[i];
            vec3 diffuse = directionalLightIntensity[i] * max(dot(normalEye, lightDirN), 0.0f) * directionalLightColor[i];
            vec3 reflectDir = reflect(-lightDirN, normalEye);
            float specCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
            vec3 specular = directionalLightIntensity[i] * specularStrength * specCoeff * directionalLightColor[i];
            if (directionalLightIsShadowCasting[i] != 0) {
                float shadow = computeDirectionalAndSpotLightShadow(true, i);
                diffuse *= 1.0f - shadow;
                specular *= 1.0f - shadow;
            }
            directionalAmbientTotal += ambient;
            directionalDiffuseTotal += diffuse;
            directionalSpecularTotal += specular;
        }
    }
}

void computePointLights()
{
    for (int i = 0; i < 10; i++)
    {
        if (pointLightIntensity[i] > 0.0f) {
            vec3 lightPosEye = vec3(viewMatrix * vec4(pointLightPosition[i], 1.0f));
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
}

void computeSpotLights()
{
    for (int i = 0; i < 10; i++)
    {
        if (spotLightIntensity[i] > 0.0f) {
            vec3 lightPosEye = vec3(viewMatrix * vec4(spotLightPosition[i], 1.0f));
            vec3 lightVector = lightPosEye - fPosEye.xyz;
            float dist = length(lightVector);
            lightVector = normalize(lightVector);
            vec3 spotDir = normalize(vec3(viewMatrix * vec4(spotLightDirection[i], 0.0f)));
            float angle = max(dot(spotDir, -lightVector), 0.0f);
            if (angle > spotLightCutoff[i])
            {
                float fade = 1.0 - (1.0 - angle) / (1.0 - spotLightCutoff[i]);
                float attenuation = 1.0f / (constant + linear * dist + quadratic * (dist * dist));
                vec3 ambient = attenuation * fade * spotLightIntensity[i] * ambientStrength * spotLightColor[i];
                vec3 diffuse = attenuation * fade * spotLightIntensity[i] * max(dot(normalEye, lightVector), 0.0f) * spotLightColor[i];
                vec3 reflectDir = reflect(-lightVector, normalEye);
                float specCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
                vec3 specular = attenuation * fade * spotLightIntensity[i] * specularStrength * specCoeff * spotLightColor[i];
            
                if (spotLightIsShadowCasting[i] != 0) {
                    float shadow = computeDirectionalAndSpotLightShadow(false, i);
                    diffuse *= 1.0f -  shadow;
                    specular *= 1.0f - shadow;
                }

                spotAmbientTotal += ambient;
                spotDiffuseTotal += diffuse;
                spotSpecularTotal += specular;
            }
        }
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
    computeSpotLights();

    vec3 diffuseTextureColor = texture(diffuseTexture, fTexCoords).rgb;
    vec3 specularTextureColor = texture(specularTexture, fTexCoords).rgb;

    vec3 finalDirectional = min((directionalAmbientTotal + directionalDiffuseTotal) * 
                     diffuseTextureColor + directionalSpecularTotal * specularTextureColor, 1.0f);

    vec3 finalPoint = min((pointAmbientTotal + pointDiffuseTotal) * 
                     diffuseTextureColor + pointSpecularTotal * specularTextureColor, 1.0f);

    vec3 finalSpot = min((spotAmbientTotal + spotDiffuseTotal) *
                    diffuseTextureColor + spotSpecularTotal * specularTextureColor, 1.0f);

    vec3 color = min(finalDirectional + finalPoint + finalSpot, 1.0f);
    fColor = vec4(color, 1.0f);
    // fColor = vec4(shadow, 0, 0, 1.0f);
    // fColor = vec4(fPositionLight.xyz / fPositionLight.w, 1.0f);
}
