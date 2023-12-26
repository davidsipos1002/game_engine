#pragma once

#include <engine/GeneralIncludes.hpp>
#include <objects/Model3D.hpp>
#include <animation/Animated.hpp>

namespace gps
{
    class Entity : public Animated<Entity>
    {
    private:
        Model3D *model;

    public:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        float ambientStrength;
        float specularStrength;

        Entity(Model3D *model) : model(model), position(glm::vec3(0, 0, 0)), rotation(glm::vec3(0, 0, 0)), scale(glm::vec3(1)) {}

        Model3D *getModel();
    };
}