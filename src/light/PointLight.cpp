#include <light/PointLight.hpp>

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
        shader->loadValue("pointLightIsShadowCasting[" + std::to_string(i) + "]", isShadowCasting);
    }

    void PointLight::calculateLightMatrices(int shadowWidth, int shadowHeight)
    {
        float aspect = 1.0f;
        if (shadowHeight)
            aspect = (float)shadowWidth / shadowHeight;
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 0.1f, 20.0f);

        lightSpaceMatrices[0] = projection *
                                glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
        lightSpaceMatrices[1] = projection * 
                                glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0));
        lightSpaceMatrices[2] = projection * 
                                glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
        lightSpaceMatrices[3] = projection * 
                                glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0));
        lightSpaceMatrices[4] = projection * 
                                glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0));
        lightSpaceMatrices[5] = projection * 
                                glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0));
    }
    
   const glm::mat4 &PointLight::getLightMatrix(int i)
   {
        return lightSpaceMatrices[i];
   }
}