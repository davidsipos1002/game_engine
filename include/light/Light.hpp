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
        virtual void calculateLightMatrices(int shadowWidth = 0, int shadowHeight = 0) = 0;
        virtual const glm::mat4 &getLightMatrix(int i = 0) = 0;
    };
}