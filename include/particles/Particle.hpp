#pragma once

#include <engine/GeneralIncludes.hpp>

namespace gps
{
    class ParticleRenderer;

    class Particle
    {
    private:
        friend class ParticleRenderer;

        glm::vec3 position;
        glm::vec3 velocity;
        float gravity;
        float lifeLength;
        float rotation;
        float scale;
        double elapsedTime;

    public:
        Particle(const glm::vec3 &position, const glm::vec3 &velocity, float gravity,
                 float lifeLength, float rotation, float scale);

        bool update(double delta);
    };
}