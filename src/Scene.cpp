#include <Scene.hpp>

Scene::Scene(bool fullScreen, int width, int height, const std::string &title) : Application(fullScreen, width, height, title)
{
}

void Scene::init()
{
    std::vector<std::string> filenames;
    filenames.push_back("models/particleStar.png");
    filenames.push_back("models/star1.png");
    filenames.push_back("models/star2.png");
    ParticleTexture *texture = loader->loadParticleTexture(filenames, true);
    emitter = new ParticleEmitter(particleManager, texture, glm::vec3(-2, 2, 0), 500, glm::vec3(1.5, 3, 1.5), -5, 1, 3);

    Entity *teapot = loader->loadEntity("models/teapot/teapot20segUT.obj", teapot1);
    teapot->position = glm::vec3(0, 0, 0);
    teapot->rotation = glm::vec3(0, 0, 0);
    teapot->scale = glm::vec3(1, 1, 1);
    teapot->ambientStrength = 0.2f;
    teapot->specularStrength = 0.5f;
    renderer->addEntity(teapot);

    teapot = loader->loadEntity("models/ground/ground.obj", teapot2);
    teapot->position = glm::vec3(0, -1, 0);
    teapot->rotation = glm::vec3(0, 3.14f / 2, 0);
    teapot->scale = glm::vec3(1, 1, 1);
    teapot->ambientStrength = 0.2f;
    teapot->specularStrength = 0.5f;
    renderer->addEntity(teapot);

    teapot = loader->loadEntity("models/ground/ground.obj", teapot2);
    teapot->position = glm::vec3(3, 0, 0);
    teapot->rotation = glm::vec3(0, 0, 3.14f / 2);
    teapot->scale = glm::vec3(1, 1, 1);
    teapot->ambientStrength = 0.2f;
    teapot->specularStrength = 0.5f;
    renderer->addEntity(teapot);

    teapot = loader->loadEntity("models/nanosuit/nanosuit.obj", teapot2);
    teapot->position = glm::vec3(0, 0, 0);
    // teapot->rotation = glm::vec3(0, 3.14f / 2, 0);
    teapot->scale = glm::vec3(1, 1, 1);
    teapot->ambientStrength = 0.2f;
    teapot->specularStrength = 0.5f;
    renderer->addEntity(teapot);

    loader->loadEntity("models/quad/quad.obj", quadEntity);

    DirectionalLight &light = renderer->getDirectionalLight(0);
    light.intensity = 1.0f;
    light.lightColor = glm::vec3(1, 1, 1);
    light.lightDirection = glm::vec3(0, 1, 1);
    light.isShadowCasting = true;

    DirectionalLight &light1 = renderer->getDirectionalLight(1);
    light1.intensity = 0.0f;
    light1.lightColor = glm::vec3(0, 1, 0);
    light1.lightDirection = glm::vec3(0, 2, 3);
    // light1.isShadowCasting = true;

    PointLight &pointLight = renderer->gePointLight(0);
    pointLight.intensity = 1.0f;
    pointLight.lightColor = glm::vec3(0, 0, 1);
    pointLight.lightPosition = glm::vec3(-3, 2, 0);
    pointLight.isShadowCasting = true;

    PointLight &pointLight2 = renderer->gePointLight(1);
    pointLight2.intensity = 0.0f;
    pointLight2.lightColor = glm::vec3(1, 1, 0);
    pointLight2.lightPosition = glm::vec3(3, 2, 0);

    SpotLight &spotLight = renderer->getSpotLight(0);
    spotLight.intensity = 0.0f;
    spotLight.lightPosition = glm::vec3(0.75, 3, 0);
    spotLight.lightDirection = glm::vec3(0, -1, 0);
    spotLight.lightColor = glm::vec3(1, 1, 1);
    spotLight.cutoff = 3.14f / 10;
    // spotLight.isShadowCasting = true;

    Animation<Entity> *animation = animator->createTriggeredAnimation<Entity>([&]()
                                                                             { return keyboard->isKeyPressed(GLFW_KEY_P); },
                                                                             true);
    Animation<Entity> *teapotA = animator->createPeriodicAnimation<Entity>(3, 3);
    Animation<Entity> *subComponent = animator->createTriggeredAnimation<Entity>([&]()
                                                                                { return keyboard->isKeyPressed(GLFW_KEY_P); },
                                                                                true);
    animation->addKeyFrame(KeyFrame<Entity>());
    animation->addKeyFrame(KeyFrame<Entity>(1.5, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
    subComponent->addKeyFrame(KeyFrame<Entity>());
    subComponent->addKeyFrame(KeyFrame<Entity>(1.5, glm::vec3(0, 0, 0), glm::vec3(0, 3.14f, 0), glm::vec3(1, 1, 1)));
    teapotA->addKeyFrame(KeyFrame<Entity>());
    teapotA->addKeyFrame(KeyFrame<Entity>(1.5, glm::vec3(-5, 0, 0), glm::vec3(0, 3.14f, 0), glm::vec3(1, 1, 1)));
    teapotA->addKeyFrame(KeyFrame<Entity>(2.5, glm::vec3(-5, 3, 0), glm::vec3(0, 2 * 3.14f, 0), glm::vec3(1, 1, 1)));
    loader->getEntity(this->teapot1)->attachMainComponentAnimation(teapotA);
    loader->getEntity(teapot2)->attachMainComponentAnimation(animation);
    loader->getEntity(teapot2)->attachSubComponentAnimation(subComponent, "Helmet_Helmet");
    loader->getEntity(teapot2)->attachSubComponentAnimation(subComponent, "Visor_Glass");
    renderer->enableSkyBox = true;
    renderer->fogDensity = 0.02f;
    particleRenderer->init();
    particleRenderer->fogDensity = 0.02f;
    Animation<Camera> *camAnim = animator->createTriggeredAnimation<Camera>([&]()
                                                                           { return keyboard->isKeyPressed(GLFW_KEY_Z); },
                                                                           false);
    camAnim->addKeyFrame(KeyFrame<Camera>(0, 1, 0.2, -M_PI, 0));
    camAnim->addKeyFrame(KeyFrame<Camera>(2, 0, 0, M_PI / 2, 0));
    camera->setAnimation(camAnim);
}

void Scene::update(double delta)
{
    emitter->emitParticles(delta);
}

bool Scene::doCustomRender()
{
    return keyboard->isKeyPressed(GLFW_KEY_M);
}

void Scene::render()
{
    renderer->displayDirectionalAndSpotLightShadowMap(false, loader->getEntity(quadEntity), 0);
}

void Scene::cleanup()
{
    delete emitter;
}