#pragma once

#include <engine/GeneralIncludes.hpp>
#include <object/Model3D.hpp>
#include <animation/Animation.hpp>
#include <object/Entity.hpp>
#include <graphics/Shader.hpp>
#include <object/Camera.hpp>
#include <light/DirectionalLight.hpp>
#include <light/PointLight.hpp>
#include <light/SpotLight.hpp>
#include <graphics/ShadowMap.hpp>
#include <graphics/Window.h>
#include <unordered_map>
#include <array>

namespace gps
{
    class Renderer
    {
    private:
        std::unordered_map<Model3D *, std::vector<Entity *>> entities;
        Window *window;

        void renderModels(Model3D *model, Shader *shader);
        void renderDirectionalLightShadowMapModels(Model3D *model, Shader *shader);
        void renderDirectionalLightShadowMapEntities(int index, Shader *shader, ShadowMap *shadowMap);
        void getEntityAnimationMatrices(Animation<Entity> *animation, glm::mat4 &translate, glm::mat4 &rotate, glm::mat4 &scale);

    public:
        std::array<DirectionalLight, 3> directionalLights;
        std::array<PointLight, 10> pointLights;
        std::array<SpotLight, 10> spotLights;

        Renderer(Window *window);

        void addEntity(Entity *entity);
        void renderEntities(Camera *camera, Shader *shader, glm::mat4 projectionMatrix, ShadowMap *shadow);
        void renderShadowMap(Entity *quad, Shader *shader, ShadowMap *shadowMap);
    };
}