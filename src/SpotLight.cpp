#include <SpotLight.hpp>

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
        shader->loadValue("spotLightCutoff[" + std::to_string(i) + "]", cutoff);
        shader->loadValue("spotLightIntensity[" + std::to_string(i) + "]", intensity);
    }
}