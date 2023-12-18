#pragma once

#include <GeneralIncludes.hpp>
#include <Model3D.hpp>
#include <Animation.hpp>
#include <Entity.hpp>
#include <Shader.hpp>
#include <Camera.hpp>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>
#include <SpotLight.hpp>
#include <unordered_map>
#include <array>

namespace gps
{
    class Renderer
    {
    private:
        std::unordered_map<Model3D *, std::vector<Entity *>> entities;

        void renderModels(Model3D *model, Shader *shader);
        void getEntityAnimationMatrices(Animation<Entity> *animation, glm::mat4 &translate, glm::mat4 &rotate, glm::mat4 &scale);

    public:
        std::array<DirectionalLight, 3> directionalLights;
        std::array<PointLight, 10> pointLights;
        std::array<SpotLight, 10> spotLights;

        void addEntity(Entity *entity);
        void renderEntities(Camera *camera, Shader *shader, glm::mat4 projectionMatrix);
    };
}