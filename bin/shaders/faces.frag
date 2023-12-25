#version 410 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

in vec3 fPosition;
in vec3 fNormal;

vec3 ambient;
vec3 diffuse;
vec3 specular;

float ambientStrength = 0.2f;
float specularStrength = 0.5f;
vec3 baseColor = vec3(0.5f, 0.5f, 0.5f);
vec3 lightDirection = vec3(0, 1, 1);
vec3 lightColor = vec3(1, 1, 1); 

mat3 normalMatrix;
vec4 fPosEye;
vec3 normalEye;
vec3 viewDir;

out vec4 fColor;

void computeDirectionalLight()
{
    vec3 lightDirN = vec3(normalize(viewMatrix * vec4(lightDirection, 0.0f)));
    ambient = ambientStrength * lightColor;
    diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
    vec3 reflectDir = reflect(-lightDirN, normalEye);
    float specCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    specular = specularStrength * specCoeff * lightColor;
}

void main()
{
    normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
    fPosEye = viewMatrix * modelMatrix * vec4(fPosition, 1.0f); 
    normalEye = normalize(normalMatrix * fNormal);
    viewDir = normalize(-fPosEye.xyz);
    computeDirectionalLight();
    vec3 color = min((ambient + diffuse) * baseColor + specular, 1.0f);
    fColor = vec4(color, 1.0f);
}