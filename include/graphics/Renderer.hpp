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
#include <io/Loader.hpp>
#include <object/SkyBox.hpp>
#include <unordered_map>
#include <array>

namespace gps
{
    class Renderer
    {
    private:
        std::unordered_map<Model3D *, std::vector<Entity *>> entities;
        std::array<std::pair<DirectionalLight, ShadowMap *>, 3> directionalLights;
        std::array<std::pair<PointLight, ShadowMap *>, 10> pointLights;
        std::array<std::pair<SpotLight, ShadowMap *>, 10> spotLights;
        Window *window;
        Shader *entityShader;
        Shader *directionalAndSpotShadowShader;
        Shader *pointShadowShader;
        Shader *shadowMapShader;
        Shader *skyboxShader;
        SkyBox *skybox;

        void renderModels(Model3D *model);
        void renderShadowMapModels(Model3D *model, Shader *shader);
        void renderDirectionalAndSpotLightShadowMapEntities(Light &light, ShadowMap *map);
        void renderPointLightShadowMapEntities(PointLight &light, ShadowMap *map);
        void __renderShadowMap(Entity *quad, ShadowMap *shadowMap);
        void renderShadowMaps();
        void loadShadowMaps(Shader *shader);
        void getEntityAnimationMatrices(Animation<Entity> *animation, glm::mat4 &translate, glm::mat4 &rotate, glm::mat4 &scale);

    public:
        Renderer(Window *window, Loader *loader);
        ~Renderer();

        void addEntity(Entity *entity);
        void displayDirectionalAndSpotLightShadowMap(Entity *quad, int i);
        void renderEntities(Camera *camera, glm::mat4 projectionMatrix);
        DirectionalLight &getDirectionalLight(int index);
        PointLight &gePointLight(int index);
        SpotLight &getSpotLight(int index);
    };
}