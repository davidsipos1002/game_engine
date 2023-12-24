#include <light/DirectionalLight.hpp>

namespace gps
{
    DirectionalLight::DirectionalLight(glm::vec3 lightDirection,
                                       glm::vec3 lightColor, float intensity) : lightDirection(lightDirection),
                                                               lightColor(lightColor), intensity(intensity) {}
    
    DirectionalLight::~DirectionalLight() {}
    
    void DirectionalLight::loadUniforms(Shader *shader, int i)
    {
        shader->loadVector("directionalLightDirection[" + std::to_string(i) + "]", lightDirection); 
        shader->loadVector("directionalLightColor[" + std::to_string(i) + "]", lightColor); 
        shader->loadValue("directionalLightIntensity[" + std::to_string(i) + "]", intensity); 
        shader->loadValue("directionalLightIsShadowCasting[" + std::to_string(i) + "]", isShadowCasting); 
        shader->loadMatrix("directionalLightSpaceMatrix[" + std::to_string(i) + "]", lightSpaceMatrix); 
    }

    void DirectionalLight::calculateLightMatrices(int shadowWidth, int shadowHeight) 
    {
        glm::mat4 viewMatrix = glm::lookAt(4.0f * lightDirection, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        const GLfloat near_plane = 0.1f, far_plane = 20.0f;
        glm::mat4 orthographicMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightSpaceMatrix = orthographicMatrix * viewMatrix;
    }

    const glm::mat4 &DirectionalLight::getLightMatrix(int i)
    {
        return lightSpaceMatrix;
    }
}