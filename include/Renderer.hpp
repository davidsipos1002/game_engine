#pragma once

#include <GeneralIncludes.hpp>
#include <Model3D.hpp>
#include <Entity.hpp>
#include <Shader.hpp>
#include <Camera.hpp>
#include <DirectionalLight.hpp>
#include <unordered_map>
#include <array>

namespace gps
{
    class Renderer
    {
    private:
        std::unordered_map<Model3D *, std::vector<Entity *>> entities;

        void renderModels(Model3D *model, Shader *shader);

    public:
        std::array<DirectionalLight, 3> directionalLights;

        void addEntity(Entity *entity);
        void renderEntities(Camera *camera, Shader *shader, glm::mat4 projectionMatrix);
    };
}