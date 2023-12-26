#pragma once

#include <engine/GeneralIncludes.hpp>
#include <animation/Animated.hpp>
#include <lights/Light.hpp>
#include <array>
namespace gps
{
    class PointLight : public Light, Animated<PointLight>
    {
    public:
        glm::vec3 lightPosition;
        glm::vec3 lightColor;
        float intensity = 0;
        std::array<glm::mat4, 6> lightSpaceMatrices;

        PointLight() {}
        PointLight(const glm::vec3 &lightPosition, const glm::vec3 &lightColor, float intensity);
        ~PointLight(){};
        void loadUniforms(Shader *shader, int i) override;
        void calculateLightMatrices(int shadowWidth = 0, int shadowHeight = 0) override;
        const glm::mat4 &getLightMatrix(int i = 0) override;
    };
}
