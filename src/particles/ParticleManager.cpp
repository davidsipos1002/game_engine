#include <particles/ParticleManager.hpp>

namespace gps
{
    void ParticleManager::update(double delta)
    {
        for (std::unordered_map<ParticleTexture*, std::vector<Particle>>::iterator it = particles.begin(); it != particles.end();)
        {
            for (std::vector<Particle>::iterator pit = it->second.begin(); pit != it->second.end();)
            {
                if(!pit->update(delta))
                    pit = it->second.erase(pit);
                else
                    pit++;
            }
            if (it->second.size())
                it++;
            else
                it = particles.erase(it);
        }
    }

    void ParticleManager::addParticle(ParticleTexture *texture, const Particle &particle)
    {
        particles[texture].push_back(particle);
    }
}