#include <particles/ParticleEmitter.hpp>
#include <cmath>
#include <iostream>

namespace gps
{
    ParticleEmitter::ParticleEmitter(ParticleManager *manager, const glm::vec3 &position,
                                     float particlesPerSecond, const glm::vec3 &speed,
                                     float gravity, float lifeLength) : manager(manager), position(position),
                                                                        particlesPerSecond(particlesPerSecond),
                                                                        speed(speed), gravity(gravity), lifeLength(lifeLength)
    {
        std::random_device rd;
        engine = std::mt19937(rd());
        distribution = std::uniform_real_distribution<float>(0, 1);
    }

    void ParticleEmitter::emitParticles(double delta)
    {
        float particlesToCreate = particlesPerSecond * delta;
        int count = std::floor(particlesToCreate);
        float partialParticle = particlesToCreate - count;
        for (int i = 0; i < count; i++)
            generateParticle();
        if (distribution(engine) < partialParticle)
            generateParticle();
    }

    void ParticleEmitter::generateParticle()
    {
        float x = (distribution(engine) * 2 - 1) * distribution(engine);
        float y = (distribution(engine) * 2 - 1) * distribution(engine);
        glm::vec3 v = glm::normalize(glm::vec3(x, 1, y));
        v *= speed;
        float rotation = distribution(engine) * M_PI;
        float scale = distribution(engine) / 10;
        manager->addParticle(Particle(position, v, gravity, lifeLength, rotation, scale));
    }
}