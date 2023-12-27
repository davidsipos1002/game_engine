#pragma once

#include <engine/GeneralIncludes.hpp>
#include <graphics/Shader.hpp>
#include <io/stb_image.h>

#include <vector>
#include <string>
#include <stdio.h>

namespace gps
{
    class SkyBox
    {
    public:
        SkyBox(const std::vector<std::string> &skyBoxPath);
        ~SkyBox();
        void render(Shader *shader, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix, const glm::vec3 &colorModifier);

    private:
        GLuint skyboxVAO;
        GLuint skyboxVBO;
        GLuint cubemapTexture;
    };
}