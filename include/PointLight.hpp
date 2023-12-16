#pragma once

#include <GeneralIncludes.hpp>
#include <Light.hpp>

namespace gps
{
    class PointLight : public Light { 
        public:
            glm::vec3 lightPosition;
            glm::vec3 lightColor;
            float intensity;

            PointLight() {}
            PointLight(glm::vec3 lightPosition, glm::vec3 lightColor, float intensity);
            ~PointLight() {};
            void loadUniforms(Shader *shader, int i);
    };
}
