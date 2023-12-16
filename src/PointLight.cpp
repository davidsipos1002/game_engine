#include <PointLight.hpp>

namespace gps
{
    PointLight::PointLight(glm::vec3 lightPosition,
                           glm::vec3 lightColor, float intensity) : lightPosition(lightPosition),
                                                                    lightColor(lightColor), intensity(intensity) {}
    
    void PointLight::loadUniforms(Shader *shader, int i)
    {
        shader->loadVector("pointLightPosition[" + std::to_string(i) + "]", lightPosition);
        shader->loadVector("pointLightColor[" + std::to_string(i) + "]", lightColor);
        shader->loadValue("pointLightIntensity[" + std::to_string(i) + "]", intensity);
    }
}