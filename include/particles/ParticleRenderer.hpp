#pragma once

#include <engine/GeneralIncludes.hpp>
#include <particles/Particle.hpp>
#include <particles/ParticleManager.hpp>
#include <particles/ParticleTexture.hpp>
#include <io/Loader.hpp>
#include <graphics/Shader.hpp>
#include <objects/Camera.hpp>
#include <vector>

namespace gps
{
    class ParticleRenderer
    {
    private:
        ParticleManager *manager;
        Shader *particleShader;
        GLuint vaoID;
        GLuint vboID;

        void loadParticleTexture(ParticleTexture *texture);
        void unloadParticleTexture(ParticleTexture *texture);
        void getTextureData(ParticleTexture *texture, const Particle &particle, int &i, int &j, float &blendFactor);
        void loadModelMatrix(const Particle &particle, Camera *camera);

    public:
        ParticleRenderer(ParticleManager *manager, Loader *loader);

        void init();
        void render(Camera *camera);
    };
}