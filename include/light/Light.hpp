#pragma once

#include <graphics/Shader.hpp>

namespace gps
{
    class Light
    {
    public:
        bool isShadowCasting = false;
        virtual ~Light(){};

        virtual void loadUniforms(Shader *shader, int i) = 0;
        virtual void calculateLightMatrices(){};
        virtual glm::mat4 getLightMatrix(int i = 0) { return glm::mat4(1); }
    };
}