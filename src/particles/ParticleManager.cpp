#include <particles/ParticleManager.hpp>

namespace gps
{
    void ParticleManager::update(double delta)
    {
        for (std::vector<Particle>::iterator it = particles.begin(); it != particles.end();)
        {
            if (!it->update(delta))
                it = particles.erase(it);
            else
                it++;
        }
    }

    void ParticleManager::addParticle(const Particle &particle)
    {
        particles.push_back(particle);
    }
}