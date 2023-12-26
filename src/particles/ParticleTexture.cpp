#include <particles/ParticleTexture.hpp>

#include <stdexcept>

namespace gps
{
    ParticleTexture::ParticleTexture(const std::vector<std::tuple<uint8_t *, int, int>> &imageData, bool additive) : additive(additive)
    {
        int count = imageData.size() > 8 ? 8 : imageData.size();
        for (int i = 0; i < count; i++)
        {
            GLuint tex;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, std::get<1>(imageData[i]),
                         std::get<2>(imageData[i]), 0, GL_RGBA, GL_UNSIGNED_BYTE, std::get<0>(imageData[i]));
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D, 0);
            textures.push_back(tex);
        }
    }
    
    ParticleTexture::~ParticleTexture()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        for (auto &tex : textures)
            glDeleteTextures(1, &tex);
    }
    
    GLuint ParticleTexture::getTexture(int i)
    {
        if (i >= textures.size())
            throw std::runtime_error("Invalid index");
        return textures[i];
    }
    
    int ParticleTexture::getCount()
    {
        return textures.size();
    }
    
    bool ParticleTexture::isAdditive()
    {
        return additive;
    }
}