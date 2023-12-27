#include <Scene.hpp>

Scene::Scene(bool fullScreen, int width, int height, const std::string &title) : Application(fullScreen, width, height, title)
{
}

void Scene::init()
{
    skyBox = loader->loadSkyBox("nightsky");
    std::vector<std::string> filenames;
    filenames.push_back("fire_particles/1.png");
    filenames.push_back("fire_particles/2.png");
    filenames.push_back("fire_particles/3.png");
    filenames.push_back("fire_particles/4.png");

    renderer->getDirectionalLight(0).intensity = 0.0f;
    renderer->getDirectionalLight(0).lightColor = glm::vec3(1);
    renderer->getDirectionalLight(0).lightDirection = glm::vec3(0, 1, 1);
    renderer->getDirectionalLight(0).isShadowCasting = false;
    
    renderer->getSpotLight(0).intensity = 0.0f;
    renderer->getSpotLight(0).lightPosition = glm::vec3(-3, 1.5, 2);
    renderer->getSpotLight(0).lightColor = glm::vec3(1, 0, 0);
    renderer->getSpotLight(0).lightDirection = glm::vec3(0, -1, 0);
    renderer->getSpotLight(0).cutoff = M_PI / 6;
    renderer->getSpotLight(0).isShadowCasting = true;
    
    renderer->getPointLight(0).intensity = 2.0f;
    renderer->getPointLight(0).lightPosition = glm::vec3(-3, 1, 2);
    renderer->getPointLight(0).lightColor = glm::vec3(0, 0, 1);
    renderer->getPointLight(0).isShadowCasting = true;

    generateGround(5, 5);
    generateTreesAndMushrooms(5, 5);
    Entity *cabin = loader->loadEntity("models/forest_cabin/forest_cabin.obj");
    cabin->position = glm::vec3(0, -1, 0);
    cabin->scale = glm::vec3(0.1, 0.1, 0.1);
    cabin->ambientStrength = 0.2f;
    cabin->specularStrength = 0.5f;
    renderer->addEntity(cabin, false);
    
    
    quad = loader->loadEntity("models/quad/quad.obj");

    ParticleTexture *texture = loader->loadParticleTexture(filenames, false);
    emitter = new ParticleEmitter(particleManager, texture, glm::vec3(1, -1, 1), 200, glm::vec3(0.5, 0.5, 0.5), 0, 0.5, 0.5);
    emitter1 = new ParticleEmitter(particleManager, texture, glm::vec3(1, -1, 1), 200, glm::vec3(0.5, 0.5, 0.5), 0, 0.5, 0.5);
    renderer->fogColor = 1 / 255.0f * glm::vec3(12, 16, 31);
    renderer->fogDensity = 0.04f;
    renderer->skyBoxColorModifier = glm::vec3(0.2, 0.2, 0.2);
}

void Scene::generateGround(int x, int y)
{
    for (int i = -x / 2; i < x / 2; i++)
    {
        for (int j = -y / 2; j < y / 2; j++)
        {
            Entity *tile = loader->loadEntity("models/ground/ground.obj");
            tile->position = glm::vec3(i * 2, -1, j * 2);
            tile->scale = glm::vec3(0.1, 0.1, 0.1);
            tile->ambientStrength = 0.2f;
            tile->specularStrength = 0.5f;
            renderer->addEntity(tile, true);
        }
    }
}

void Scene::generateTreesAndMushrooms(int x, int y)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = -x / 2; i < x / 2; i++)
    {
        for (int j = -y / 2; j < y / 2; j++)
        {
            if (i != 0 && j != 0)
            {
                std::uniform_real_distribution<> pos(-1, 1);
                Entity *tree = loader->loadEntity("models/tree/treePBR.obj");
                tree->position = glm::vec3(i * 2 + pos(gen), -1, j * 2 + pos(gen));
                tree->scale = glm::vec3(0.1, 0.1, 0.1);
                tree->ambientStrength = 0.2f;
                tree->specularStrength = 0.5f;
                renderer->addEntity(tree, true);

                Entity *mushroom = loader->loadEntity("models/mushrooms/Pilz.obj");
                mushroom->position = glm::vec3(i * 2, -1, j * 2);
                mushroom->scale = glm::vec3(0.1, 0.1, 0.1);
                mushroom->ambientStrength = 0.2f;
                mushroom->specularStrength = 0.5f;
                renderer->addEntity(mushroom, true);
            }
        }
    }
}

void Scene::update(double delta)
{
    emitter->emitParticles(delta);
    emitter1->emitParticles(delta);
}

bool Scene::doCustomRender()
{
    return keyboard->isKeyPressed(GLFW_KEY_M);
}

void Scene::render()
{
    renderer->displayDirectionalAndSpotLightShadowMap(true, quad, 0);
}

void Scene::cleanup()
{
}