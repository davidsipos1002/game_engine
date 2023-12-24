#include <light/SpotLight.hpp>

namespace gps
{

    SpotLight::SpotLight(glm::vec3 lightPosition, glm::vec3 lightDirection,
                         glm::vec3 lightColor, float cutoff, float intensity) : lightPosition(lightPosition),
                                                                                lightDirection(lightDirection),
                                                                                lightColor(lightColor), cutoff(cutoff),
                                                                                intensity(intensity) {}

    void SpotLight::loadUniforms(Shader *shader, int i)
    {
        shader->loadVector("spotLightPosition[" + std::to_string(i) + "]", lightPosition);
        shader->loadVector("spotLightDirection[" + std::to_string(i) + "]", lightDirection);
        shader->loadVector("spotLightColor[" + std::to_string(i) + "]", lightColor);
        shader->loadValue("spotLightCutoff[" + std::to_string(i) + "]", cos(cutoff));
        shader->loadValue("spotLightIntensity[" + std::to_string(i) + "]", intensity);
        shader->loadValue("spotLightIsShadowCasting[" + std::to_string(i) + "]", isShadowCasting);
        shader->loadMatrix("spotLightSpaceMatrix[" + std::to_string(i) + "]", lightSpaceMatrix);
    }
    
    void SpotLight::calculateLightMatrices()
    {
        glm::mat4 viewMatrix = glm::lookAt(lightPosition, lightDirection, glm::normalize(glm::cross(lightDirection, lightPosition)));
        glm::mat projectionMatrix = glm::perspective(cutoff, 1.0f, 0.1f, 20.0f);
        lightSpaceMatrix = projectionMatrix * viewMatrix;
    }
    
    glm::mat4 SpotLight::getLightMatrix(int i)
    {
        return lightSpaceMatrix;
    }
}