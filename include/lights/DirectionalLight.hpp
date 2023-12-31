#pragma once

#include <engine/GeneralIncludes.hpp>
#include <animation/Animated.hpp>
#include <lights/Light.hpp>

namespace gps
{
    class DirectionalLight : public Light, public Animated<DirectionalLight>
    {
    public:
        glm::mat4 lightSpaceMatrix = glm::mat4(1);
        glm::vec3 lightDirection;
        glm::vec3 lightColor;
        float intensity = 0;

        DirectionalLight() {}
        DirectionalLight(const glm::vec3 &lightDirection, const glm::vec3 &lightColor, float intensity);
        ~DirectionalLight();
        void loadUniforms(Shader *shader, int i) override;
        void calculateLightMatrices(int shadowWidth = 0, int shadowHeight = 0) override;
        const glm::mat4 &getLightMatrix(int i = 0) override;
    };
}