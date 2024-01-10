#include <Scene.hpp>

Scene::Scene(bool fullScreen, int width, int height, const std::string &title) : Application(fullScreen, width, height, title)
{
}

void Scene::init()
{
    skyBox = loader->loadSkyBox("nightsky");
    quad = loader->loadEntity("models/quad/quad.obj");

    generateGround(30, 30);
    generateTrees(10, 10, glm::vec3(-1, 0, -1));
    setupCampingPlace(glm::vec3(7, 0, 7));
    setupForestCabin(glm::vec3(0, 0, 0));
    
    PointLight &forestLight = renderer->getPointLight(2);
    forestLight.intensity = 0.8f;
    forestLight.lightPosition = glm::vec3(-10, 1, -8);
    forestLight.lightColor = glm::vec3(0, 0, 1);
    forestLight.isShadowCasting = true;
    
    Entity *nanosuit = loader->loadEntity("models/nanosuit/nanosuit.obj");
    nanosuit->position = glm::vec3(-12, 0, -10);
    renderer->addEntity(nanosuit, false);
    
    Animation<Entity> *animation = animator->createTriggeredAnimation<Entity>([&] () {return keyboard->isKeyPressed(GLFW_KEY_P);}, false); 
    animation->addKeyFrame(KeyFrame<Entity>());
    animation->addKeyFrame(KeyFrame<Entity>(1.5, glm::vec3(0, 0, 0), glm::vec3(0, M_PI, 0), glm::vec3(1, 1, 1)));
    animation->addKeyFrame(KeyFrame<Entity>(3, glm::vec3(0, 0, 0), glm::vec3(0, 2 * M_PI, 0), glm::vec3(1, 1, 1)));
    nanosuit->attachSubComponentAnimation(animation, "Helmet_Helmet");
    nanosuit->attachSubComponentAnimation(animation, "Visor_Glass");

    setupCameraAnimation();

    renderer->fogColor = 1 / 255.0f * glm::vec3(12, 16, 31);
    renderer->fogDensity = 0.04f;
    renderer->skyBoxColorModifier = 0.8f * renderer->fogColor;
}

void Scene::setupCameraAnimation()
{
    Animation<Camera> *cam = animator->createTriggeredAnimation<Camera>([&] () {return keyboard->isKeyPressed(GLFW_KEY_C);}, true); 
    float pitch = -M_PI;
    float yaw = 0;
    cam->addKeyFrame(KeyFrame<Camera>(0, 0, 0, -M_PI, 0));
    cam->addKeyFrame(KeyFrame<Camera>(2, 0, 0, M_PI - M_PI / 6, 0));
    cam->addKeyFrame(KeyFrame<Camera>(4, 0, 7, M_PI - M_PI / 3, 0));
    cam->addKeyFrame(KeyFrame<Camera>(6, 0, 0, M_PI - M_PI / 3, 0));
    cam->addKeyFrame(KeyFrame<Camera>(8, 1, 4, M_PI / 8, 0));
    cam->addKeyFrame(KeyFrame<Camera>(10, 0, 7, M_PI / 8, 0));
    cam->addKeyFrame(KeyFrame<Camera>(12, 0, 7, -M_PI / 2 - M_PI / 12, 0));
    cam->addKeyFrame(KeyFrame<Camera>(14, 2, 4, 0, 0));
    cam->addKeyFrame(KeyFrame<Camera>(16, 2, 2, M_PI / 12, 0));
    cam->addKeyFrame(KeyFrame<Camera>(18, 2, 2, M_PI / 2, 0));
    cam->addKeyFrame(KeyFrame<Camera>(20, 2, 2, M_PI - M_PI / 3, 0));
    cam->addKeyFrame(KeyFrame<Camera>(22, 0, 0, M_PI - M_PI / 3, 0));
    cam->addKeyFrame(KeyFrame<Camera>(24, 0, 7, M_PI, 0));
    cam->addKeyFrame(KeyFrame<Camera>(26, 3, 0, M_PI - M_PI / 2, 0));
    cam->addKeyFrame(KeyFrame<Camera>(26.5, 3, 0, M_PI - M_PI / 2, 0));
    cam->addKeyFrame(KeyFrame<Camera>(27, 3, 0, M_PI - M_PI / 2, M_PI / 2));
    camera->setAnimation(cam);
}

void Scene::setupCampingPlace(const glm::vec3 &position)
{
    Entity *deadtree_rocks = loader->loadEntity("models/deadtree/deadtree_rocks.obj");
    deadtree_rocks->position = position + glm::vec3(3, -1, 0);
    deadtree_rocks->scale = glm::vec3(1, 1, 1);
    renderer->addEntity(deadtree_rocks, false);

    Entity *firewood = loader->loadEntity("models/firewood/firewood.obj");
    firewood->position = position + glm::vec3(0, -0.85, 1);
    firewood->scale = glm::vec3(0.2, 0.2, 0.2);
    renderer->addEntity(firewood, false);

    Entity *sleepingBag = loader->loadEntity("models/sleeping_bag/sleeping_bag.obj");
    sleepingBag->position = position + glm::vec3(1, -0.96, 2.5);
    sleepingBag->scale = glm::vec3(0.2, 0.2, 0.2);
    sleepingBag->rotation = glm::vec3(0, M_PI / 2 + 0.1, 0);
    renderer->addEntity(sleepingBag, false);

    Entity *mushroom = loader->loadEntity("models/mushrooms/Mushroom.obj");
    mushroom->position = position + glm::vec3(0, -1, 0);
    mushroom->scale = glm::vec3(5, 5, 5);
    renderer->addEntity(mushroom, false);

    Entity *mushroom1 = loader->loadEntity("models/mushrooms/Pilz.obj");
    mushroom1->position = position + glm::vec3(2, -0.9, 1.1);
    mushroom1->scale = glm::vec3(0.25, 0.25, 0.25);
    renderer->addEntity(mushroom1, false);

    std::vector<std::string> filenames;
    filenames.push_back("fire_particles/1.png");
    filenames.push_back("fire_particles/2.png");
    filenames.push_back("fire_particles/3.png");
    filenames.push_back("fire_particles/4.png");
    ParticleTexture *texture = loader->loadParticleTexture(filenames, true);
    emitters.push_back(new ParticleEmitter(particleManager, texture,
                                           position + glm::vec3(0, -0.8, 1), 300, glm::vec3(1, 1.1, 1), 0, 0.8, 0.8));
    emitters.push_back(new ParticleEmitter(particleManager, texture,
                                           position + glm::vec3(0.01, -0.8, 1), 300, glm::vec3(1, 1.1, 1), 0, 0.8, 0.8));
    emitters.push_back(new ParticleEmitter(particleManager, texture,
                                           position + glm::vec3(0.01, -0.8, 1.01), 300, glm::vec3(1, 1.1, 1), 0, 0.8, 0.5));
    emitters.push_back(new ParticleEmitter(particleManager, texture,
                                           position + glm::vec3(-0.01, -0.8, 0.99), 300, glm::vec3(1, 1.1, 1), 0, 0.8, 0.5));

    campfire = &(renderer->getPointLight(0));
    campfire->intensity = 0.7f;
    campfire->lightPosition = position + glm::vec3(0, 1, 1);
    campfire->lightColor = 1 / 255.0f * glm::vec3(234, 154, 24);
    campfire->isShadowCasting = true;
    Animation<PointLight> *lightAnim = animator->createPeriodicAnimation<PointLight>(0, 0.5);
    lightAnim->addKeyFrame(KeyFrame<PointLight>(0, glm::vec3(0), glm::vec3(0), 0.7));
    lightAnim->addKeyFrame(KeyFrame<PointLight>(1.5, glm::vec3(0), glm::vec3(0), 0.2));
    lightAnim->addKeyFrame(KeyFrame<PointLight>(3, glm::vec3(0), glm::vec3(0), 0.7));
    campfire->setAnimation(lightAnim);
}

void Scene::setupForestCabin(const glm::vec3 &position)
{
    Entity *cabin = loader->loadEntity("models/forest_cabin/forest_cabin.obj");
    cabin->position = position + glm::vec3(0, -1, 0);
    cabin->scale = glm::vec3(0.1, 0.1, 0.1);
    renderer->addEntity(cabin, false);

    Entity *lampPost = loader->loadEntity("models/lamp_post/LampPost.obj");
    lampPost->position = position + glm::vec3(2, -1.01, 0);
    lampPost->scale = glm::vec3(0.3, 0.3, 0.3);
    lampPost->rotation = glm::vec3(0, M_PI, 0);
    renderer->addEntity(lampPost, false);

    Entity *lampPost1 = loader->loadEntity("models/lamp_post/LampPost.obj");
    lampPost1->position = position + glm::vec3(-2.5, -1.01, 0);
    lampPost1->scale = glm::vec3(0.3, 0.3, 0.3);
    lampPost1->rotation = glm::vec3(0, M_PI, 0);
    renderer->addEntity(lampPost1, false);

    Entity *trunk = loader->loadEntity("models/trunks/trunk2.obj");
    trunk->position = position + glm::vec3(1, -1, -3);
    trunk->scale = glm::vec3(1, 1, 1);
    renderer->addEntity(trunk, false);

    Entity *trunk1 = loader->loadEntity("models/trunks/trunk3.obj");
    trunk1->position = position + glm::vec3(-4, -1, -3.5);
    trunk1->scale = glm::vec3(2, 2, 2);
    renderer->addEntity(trunk1, false);

    SpotLight &lamp1 = renderer->getSpotLight(0);
    lamp1.intensity = 2.0f;
    lamp1.lightPosition = position + glm::vec3(2, 3, 0);
    lamp1.lightColor = 1 / 255.0f * glm::vec3(245, 150, 10);
    lamp1.lightDirection = glm::vec3(0, -1, 0);
    lamp1.cutoff = M_PI / 8;
    lamp1.isShadowCasting = true;

    SpotLight &lamp2 = renderer->getSpotLight(1);
    lamp2.intensity = 2.0f;
    lamp2.lightPosition = position + glm::vec3(-2.5, 3, 0);
    lamp2.lightColor = 1 / 255.0f * glm::vec3(245, 150, 10);
    lamp2.lightDirection = glm::vec3(0, -1, 0);
    lamp2.cutoff = M_PI / 8;
    lamp2.isShadowCasting = true;

    PointLight &behind = renderer->getPointLight(1);
    behind.intensity = 0.3f;
    behind.lightPosition = position + glm::vec3(0, 0, -3);
    behind.lightColor = 1 / 255.0f * glm::vec3(255, 140, 0);
    behind.isShadowCasting = false;

    Animation<Entity> *anim = animator->createPeriodicAnimation<Entity>(0, 0.5);
    anim->addKeyFrame(KeyFrame<Entity>(0, glm::vec3(0), glm::vec3(0, 0, 0), glm::vec3(1)));
    anim->addKeyFrame(KeyFrame<Entity>(0.25, glm::vec3(0), glm::vec3(0.01, 0, 0), glm::vec3(1)));
    anim->addKeyFrame(KeyFrame<Entity>(0.5, glm::vec3(0), glm::vec3(0, 0, 0), glm::vec3(1.01)));
    anim->addKeyFrame(KeyFrame<Entity>(0.75, glm::vec3(0), glm::vec3(-0.01, 0, 0), glm::vec3(1)));
    anim->addKeyFrame(KeyFrame<Entity>(1, glm::vec3(0), glm::vec3(0, 0, 0), glm::vec3(1)));
    lampPost->attachSubComponentAnimation(anim, "Lamp");
    lampPost1->attachSubComponentAnimation(anim, "Lamp");
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

void Scene::generateTrees(int x, int y, const glm::vec3 &center)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = -x; i <= x; i += 2)
    {
        for (int j = -y; j <= y; j += 2)
        {
            std::uniform_real_distribution<> pos(-1, 1);
            glm::vec3 position = center + glm::vec3(i * 2 + pos(gen), -0.8, j * 2 + pos(gen));
            if (glm::length(position - glm::vec3(10, 0, 10)) >= 5 && glm::length(position - glm::vec3(0, 0, 0)) >= 5)
            {
                Entity *tree = loader->loadEntity("models/deadtree/deadtree2.obj");
                tree->position = position;
                tree->scale = glm::vec3(0.008, 0.008, 0.008);
                tree->ambientStrength = 0.2f;
                tree->specularStrength = 0.5f;
                renderer->addEntity(tree, true);
            }
        }
    }
}

void Scene::update(double delta)
{
    for (auto emitter : emitters)
        emitter->emitParticles(delta);
    campfire->intensity = campfire->getAnimation()->interpolatedKeyFrame.intensity;
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
    for (auto emitter : emitters)
        delete emitter;
}