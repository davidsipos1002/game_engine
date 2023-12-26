#pragma once

#include <engine/GeneralIncludes.hpp>
#include <objects/Model3D.hpp>
#include <animation/Animation.hpp>
#include <objects/Entity.hpp>
#include <graphics/Shader.hpp>
#include <objects/Camera.hpp>
#include <lights/DirectionalLight.hpp>
#include <lights/PointLight.hpp>
#include <lights/SpotLight.hpp>
#include <graphics/ShadowMap.hpp>
#include <engine/Window.hpp>
#include <io/Loader.hpp>
#include <objects/SkyBox.hpp>
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
        Shader *facesShader;
        SkyBox *skyBox;

        void initOpenGL();
        void initLights();
        void loadShadersAndSkyBox(Loader *loader, const std::string &skyBox);
        void loadLightsToShader();
        void renderModels(Model3D *model);
        void renderShadowMapModels(Model3D *model, Shader *shader);
        void renderDirectionalAndSpotLightShadowMapEntities(Light &light, ShadowMap *map);
        void renderPointLightShadowMapEntities(PointLight &light, ShadowMap *map);
        void __renderShadowMap(Entity *quad, ShadowMap *shadowMap);
        void renderShadowMaps();
        void loadShadowMaps(Shader *shader);
        void getEntityAnimationMatrices(Animation<Entity> *animation, glm::mat4 &translate, glm::mat4 &rotate, glm::mat4 &scale);

    public:
        bool enableSkyBox = true;
        float fogDensity = 0.03f;
        glm::vec3 fogColor = glm::vec3(0.5f, 0.5f, 0.5f);

        Renderer(Window *window, Loader *loader, const std::string &skyBox);
        ~Renderer();

        void addEntity(Entity *entity);
        void displayDirectionalAndSpotLightShadowMap(bool isSpotLight, Entity *quad, int index);
        void renderEntities(Camera *camera, glm::mat4 projectionMatrix);
        void renderEntitiesWithFaces(Camera *camera, glm::mat4 projectionMatrix);
        DirectionalLight &getDirectionalLight(int index);
        PointLight &gePointLight(int index);
        SpotLight &getSpotLight(int index);
    };
}