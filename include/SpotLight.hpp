#pragma once

#include <GeneralIncludes.hpp>
#include <Light.hpp>

namespace gps
{
    class SpotLight : public Light
    {
    public:
        glm::vec3 lightPosition;
        glm::vec3 lightDirection;
        glm::vec3 lightColor;
        float cutoff = 0;
        float intensity = 0;

        SpotLight() {}
        SpotLight(glm::vec3 lightPosition, glm::vec3 lightDirection, glm::vec3 lightColor, float cutoff, float intensity);
        ~SpotLight(){};
        void loadUniforms(Shader *shader, int i);
    };
}