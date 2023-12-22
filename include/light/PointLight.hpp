#pragma once

#include <engine/GeneralIncludes.hpp>
#include <animation/Animated.hpp>
#include <light/Light.hpp>

namespace gps
{
    class PointLight : public Light, Animated<PointLight> { 
        public:
            glm::vec3 lightPosition;
            glm::vec3 lightColor;
            float intensity = 0;

            PointLight() {}
            PointLight(glm::vec3 lightPosition, glm::vec3 lightColor, float intensity);
            ~PointLight() {};
            void loadUniforms(Shader *shader, int i);
    };
}
