#include <DirectionalLight.hpp>
#include <ErrorCheck.hpp>

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
    }
}