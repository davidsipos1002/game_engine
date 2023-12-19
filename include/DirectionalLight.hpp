#pragma once

#include <GeneralIncludes.hpp>
#include <Animated.hpp>
#include <Light.hpp>

namespace gps {
    class DirectionalLight : public Light, Animated<DirectionalLight> {
        public:
            glm::vec3 lightDirection;
            glm::vec3 lightColor;
            float intensity = 0;
            
            DirectionalLight() {}
            DirectionalLight(glm::vec3 lightDirection, glm::vec3 lightColor, float intensity);
            ~DirectionalLight();
            void loadUniforms(Shader *shader, int i);
    };
}