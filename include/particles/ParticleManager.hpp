#pragma once

#include <particles/Particle.hpp>
#include <vector>

namespace gps
{
    class ParticleRenderer;

    class ParticleManager
    {
    private:
        friend class ParticleRenderer;
        std::vector<Particle> particles;

    public:
        void update(double delta);
        void addParticle(const Particle &particle);
    };
}