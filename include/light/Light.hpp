#pragma once

#include <graphics/Shader.hpp>

namespace gps
{
    class Light
    {
    public:
        virtual ~Light(){};

        virtual void loadUniforms(Shader *shader, int i) = 0;
    };
}