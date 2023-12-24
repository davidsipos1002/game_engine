#pragma once

#include <engine/GeneralIncludes.hpp>
#include <animation/Animated.hpp>
#include <light/Light.hpp>

namespace gps
{
    class SpotLight : public Light, Animated<SpotLight> 
    {
    public:
        glm::vec3 lightPosition;
        glm::vec3 lightDirection;
        glm::vec3 lightColor;
        float cutoff = 0;
        float intensity = 0;
        glm::mat4 lightSpaceMatrix;

        SpotLight() {}
        SpotLight(glm::vec3 lightPosition, glm::vec3 lightDirection, glm::vec3 lightColor, float cutoff, float intensity);
        ~SpotLight(){};
        void loadUniforms(Shader *shader, int i) override;
        void calculateLightMatrices() override;
        glm::mat4 getLightMatrix(int i) override;
    };
}