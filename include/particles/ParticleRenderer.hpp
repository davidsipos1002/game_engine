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
        struct __attribute__((packed)) ParticleInstanceData {
            float col0[4];
            float col1[4];
            float col2[4];
            float col3[4];
            int i;
            int j;
            float blendFactor;
        };

        ParticleManager *manager;
        Shader *particleShader;
        GLuint vaoID;
        GLuint vertexVboID;
        GLuint instanceVboID;

        void loadParticleTexture(ParticleTexture *texture);
        void unloadParticleTexture(ParticleTexture *texture);
        void getTextureData(ParticleTexture *texture, const Particle &particle, int &i, int &j, float &blendFactor);
        void updateModelMatrix(const Particle &particle, Camera *camera, glm::mat4 &modelMatrix);
        ParticleInstanceData *getInstanceVboContents(ParticleTexture *texture, const std::vector<Particle> &particles, Camera *camera);
        void updateInstanceVboContents(ParticleInstanceData *data, int size);

    public:
        float fogDensity = 0.03f;
        glm::vec3 fogColor = glm::vec3(0.5f, 0.5f, 0.5f);

        ParticleRenderer(ParticleManager *manager, Loader *loader);
        ~ParticleRenderer();

        void init();
        void render(Camera *camera);
    };
}