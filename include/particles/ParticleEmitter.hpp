#pragma once

#include <engine/GeneralIncludes.hpp>
#include <particles/Particle.hpp>
#include <particles/ParticleManager.hpp>
#include <random>

namespace gps
{
    class ParticleEmitter
    {
    private:
        ParticleManager *manager;
        glm::vec3 position;
        float particlesPerSecond;
        glm::vec3 speed;
        float gravity;
        float lifeLength;
        std::mt19937 engine;
        std::uniform_real_distribution<float> distribution;

        void generateParticle();

    public:
        ParticleEmitter(ParticleManager *manager, const glm::vec3 &position,
                        float particlesPerSecond, const glm::vec3 &speed, float gravity, float lifeLength);
        void emitParticles(double delta);
    };
}