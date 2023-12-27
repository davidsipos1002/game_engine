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

    renderer->getDirectionalLight(0).intensity = 0.1f;
    renderer->getDirectionalLight(0).lightColor = glm::vec3(1);
    renderer->getDirectionalLight(0).lightDirection = glm::vec3(0, 1, 1);

    generateGround(10, 10);
    generateTreesAndMushrooms(10, 10);
    Entity *cabin = loader->loadEntity("models/forest_cabin/forest_cabin.obj");
    cabin->position = glm::vec3(0, -1, 0);
    cabin->scale = glm::vec3(0.1, 0.1, 0.1);
    cabin->ambientStrength = 0.2f;
    cabin->specularStrength = 0.5f;
    renderer->addEntity(cabin);

    ParticleTexture *texture = loader->loadParticleTexture(filenames, false);
    emitter = new ParticleEmitter(particleManager, texture, glm::vec3(1, -1, 1), 200, glm::vec3(0.5, 0.5, 0.5), 0, 0.5, 0.5);
    emitter1 = new ParticleEmitter(particleManager, texture, glm::vec3(1, -1, 1), 200, glm::vec3(0.5, 0.5, 0.5), 0, 0.5, 0.5);
    renderer->fogColor = 1 / 255.0f * glm::vec3(12, 16, 31);
    renderer->fogDensity = 0.04f;
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
            renderer->addEntity(tile);
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
                renderer->addEntity(tree);

                Entity *mushroom = loader->loadEntity("models/mushrooms/Pilz.obj");
                mushroom->position = glm::vec3(i * 2, -1, j * 2);
                mushroom->scale = glm::vec3(0.1, 0.1, 0.1);
                mushroom->ambientStrength = 0.2f;
                mushroom->specularStrength = 0.5f;
                renderer->addEntity(mushroom);
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
    return false;
}

void Scene::render()
{
}

void Scene::cleanup()
{
}