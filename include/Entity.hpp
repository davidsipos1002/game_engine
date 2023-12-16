#pragma once

#include <GeneralIncludes.hpp>
#include <Model3D.hpp>

namespace gps {

    class Entity {
        private:
            Model3D *model;
            
        public:
            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scale;

            Entity(Model3D *model) : model(model) {}
            Entity(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Model3D *model);
            
            Model3D *getModel();
    };
}