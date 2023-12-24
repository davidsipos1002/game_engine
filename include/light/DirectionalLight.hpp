#pragma once

#include <engine/GeneralIncludes.hpp>
#include <animation/Animated.hpp>
#include <light/Light.hpp>

namespace gps
{
    class DirectionalLight : public Light, Animated<DirectionalLight>
    {
    public:
        glm::mat4 lightSpaceMatrix = glm::mat4(1);
        glm::vec3 lightDirection;
        glm::vec3 lightColor;
        float intensity = 0;

        DirectionalLight() {}
        DirectionalLight(glm::vec3 lightDirection, glm::vec3 lightColor, float intensity);
        ~DirectionalLight();
        void loadUniforms(Shader *shader, int i) override;
        void calculateLightMatrices(int shadowWidth = 0, int shadowHeight = 0) override;
        const glm::mat4 &getLightMatrix(int i = 0) override;
    };
}