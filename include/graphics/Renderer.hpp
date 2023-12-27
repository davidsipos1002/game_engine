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
    public:
        struct __attribute__((packed)) MatrixColumns
        {
            float col0[4];
            float col1[4];
            float col2[4];
            float col3[4];
        };
        struct __attribute__((packed)) EntityInstanceData
        {
            MatrixColumns modelMatrix;
            float lightData[2];
        };

    private:
        struct ModelMetadata
        {
            GLuint vbo;
            int bufferSize;
            EntityInstanceData *instanceData;
            std::vector<Entity *> entities;
        };

        std::unordered_map<Model3D *, std::vector<Entity *>> entities;
        std::unordered_map<Model3D *, ModelMetadata> instancedEntities;
        std::array<std::pair<DirectionalLight, ShadowMap *>, 3> directionalLights;
        std::array<std::pair<PointLight, ShadowMap *>, 10> pointLights;
        std::array<std::pair<SpotLight, ShadowMap *>, 10> spotLights;
        Window *window;

        Shader *entityShader;
        Shader *instancedEntityShader;
        Shader *directionalAndSpotShadowShader;
        Shader *instancedDirectionalAndSpotShadowShader;
        Shader *pointShadowShader;
        Shader *instancedPointShadowShader;
        Shader *shadowMapShader;
        Shader *facesShader;
        Shader *instancedFacesShader;
        Shader *skyboxShader;

        SkyBox **skyBox;
        
        void initOpenGL();
        void initLights();
        void loadShaders(Loader *loader);

        void loadLightsToShader(Shader *shader);
        void loadShadowMaps(Shader *shader);
        void getEntityAnimationMatrices(Animation<Entity> *animation, glm::mat4 &translate, glm::mat4 &rotate, glm::mat4 &scale);
        void computeEntityModelMatrix(Entity * entity, glm::mat4 &translate, glm::mat4 &rotate, glm::mat4 &scale);

        void addInstancedEntity(Entity *entity);
        void prepareInstances();
        void renderInstancedModels(Model3D* model, ModelMetadata &metadata, bool isShadowMapRender);
        void renderInstancedEntities(Camera *camera);

        void renderModels(Model3D *model, Shader *shader, bool isShadowMapRender);
        void renderEntities(Camera *camera);

        void renderDirectionalAndSpotLightShadowMapEntities(Light &light, ShadowMap *map);
        void renderPointLightShadowMapEntities(PointLight &light, ShadowMap *map);
        void __renderShadowMap(Entity *quad, ShadowMap *shadowMap);
        void renderShadowMaps();

    public:
        bool enableSkyBox = true;
        float fogDensity = 0.03f;
        glm::vec3 fogColor = glm::vec3(0.5f, 0.5f, 0.5f);
        glm::vec3 skyBoxColorModifier = glm::vec3(1.0f);

        Renderer(Window *window, Loader *loader, SkyBox **skyBox);
        ~Renderer();

        void displayDirectionalAndSpotLightShadowMap(bool isSpotLight, Entity *quad, int index);
        void render(Camera *camera);
        void renderEntitiesWithFaces(Camera *camera);
        void addEntity(Entity *entity, bool instanced);
        void cleanup();
        DirectionalLight &getDirectionalLight(int index);
        PointLight &getPointLight(int index);
        SpotLight &getSpotLight(int index);
    };
}