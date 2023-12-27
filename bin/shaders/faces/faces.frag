#version 410 core

in vec3 fPosEye;
in vec3 fNormalEye;
flat in vec3 fLightDirection;

vec3 ambient;
vec3 diffuse;
vec3 specular;

float ambientStrength = 0.2f;
float specularStrength = 0.5f;
vec3 baseColor = vec3(0.5f, 0.5f, 0.5f);
vec3 lightColor = vec3(1, 1, 1); 

vec3 viewDir;

out vec4 fColor;

void computeDirectionalLight()
{
    diffuse = max(dot(fNormalEye, fLightDirection), 0.0f) * lightColor;
    vec3 reflectDir = reflect(-fLightDirection, fNormalEye);
    float specCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    specular = specularStrength * specCoeff * lightColor;
}

void main()
{
    viewDir = normalize(-fPosEye.xyz);
    ambient = ambientStrength * lightColor;
    computeDirectionalLight();
    vec3 color = min((ambient + diffuse) * baseColor + specular, 1.0f);
    fColor = vec4(color, 1.0f);
}