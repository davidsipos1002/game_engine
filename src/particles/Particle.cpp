#include <particles/Particle.hpp>

namespace gps
{
    Particle::Particle(const glm::vec3 &position, const glm::vec3 &velocity,
                       float gravity, float lifeLength,
                       float rotation, float scale, bool additive) : position(position),
                                                                     velocity(velocity), gravity(gravity),
                                                                     lifeLength(lifeLength), rotation(rotation),
                                                                     scale(scale), additive(additive), elapsedTime(0) {}

    bool Particle::update(double delta)
    {
        velocity.y += gravity * delta;
        position += velocity * (float)delta;
        elapsedTime += delta;
        return elapsedTime < lifeLength;
    }
}