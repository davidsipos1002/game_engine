#version 410 core

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;

out vec4 fColor;

//matrices
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
//lighting
uniform vec3 directionalLightDirection[1];
uniform vec3 directionalLightColor[1];
uniform float directionalLightIntensity[1];
// textures
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

//components
vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
mat3 normalMatrix;

void computeDirLight()
{
    normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
    //compute eye space coordinates
    vec4 fPosEye = viewMatrix * modelMatrix * vec4(fPosition, 1.0f);
    vec3 normalEye = normalize(normalMatrix * fNormal);

    //normalize light direction
    vec3 lightDirN = vec3(normalize(viewMatrix * vec4(directionalLightDirection[0], 0.0f)));

    //compute view direction (in eye coordinates, the viewer is situated at the origin
    vec3 viewDir = normalize(- fPosEye.xyz);

    //compute ambient light
    ambient = directionalLightIntensity[0] * ambientStrength * directionalLightColor[0];

    //compute diffuse light
    diffuse = max(dot(normalEye, lightDirN), 0.0f) * directionalLightColor[0];

    //compute specular light
    vec3 reflectDir = reflect(-lightDirN, normalEye);
    float specCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    specular = specularStrength * specCoeff * directionalLightColor[0];
}

void main() 
{
    computeDirLight();

    //compute final vertex color
    vec3 color = min((ambient + diffuse) * texture(diffuseTexture, fTexCoords).rgb + specular * texture(specularTexture, fTexCoords).rgb, 1.0f);

    fColor = vec4(color, 1.0f);
}
