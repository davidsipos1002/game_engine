#pragma once

#include <engine/GeneralIncludes.hpp>
#include <vector>
#include <tuple>

namespace gps
{
    class ParticleTexture
    {
    private:
        std::vector<GLuint> textures;

    public:
        ParticleTexture(const std::vector<std::tuple<uint8_t *, int, int>> &imageData);
        ~ParticleTexture();

        GLuint getTexture(int i = 0);
        int getCount();
    };
}
