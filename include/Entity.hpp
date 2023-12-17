#pragma once

#include <GeneralIncludes.hpp>
#include <Model3D.hpp>
#include <Animated.hpp>

namespace gps {

    class Entity : public Animated {
        private:
            Model3D *model;
            
        public:
            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scale;
            float ambientStrength;
            float specularStrength;

            Entity(Model3D *model) : model(model) {}
            
            Model3D *getModel();
    };
}