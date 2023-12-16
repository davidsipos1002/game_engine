#include <Entity.hpp>

namespace gps
{
    Entity::Entity(glm::vec3 position, glm::vec3 rotation,
                   glm::vec3 scale, Model3D *model) : position(position),
                                                      rotation(rotation), scale(scale), model(model) {}
    

    Model3D *Entity::getModel() 
    {
        return model;
    }
}