#pragma once

#include <particles/Particle.hpp>
#include <particles/ParticleTexture.hpp>
#include <unordered_map>

namespace gps
{
    class ParticleRenderer;

    class ParticleManager
    {
    private:
        friend class ParticleRenderer;
        std::unordered_map<ParticleTexture *, std::vector<Particle>> particles;

    public:
        void update(double delta);
        void addParticle(ParticleTexture *texture, const Particle &particle);
    };
}