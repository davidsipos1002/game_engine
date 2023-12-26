#include <engine/Application.hpp>
#include <engine/ErrorCheck.hpp>
#include <chrono>
#include <tuple>

namespace gps
{
    Application::Application()
    {
    }

    Application::~Application()
    {
        delete renderer;
        delete camera;
        delete emitter;
        delete particleRenderer;
        delete particleManager;
    }

    void Application::init()
    {
        window.createWindow(true, 1024, 768, "OpenGL Project");
        window.setResizeCallback(windowResizeCallback);
        keyboard = Keyboard::getInstance(window);
        mouse = Mouse::getInstance(window);

        renderer = new Renderer(&window, &loader, "skybox");
        camera = new Camera(glm::vec3(0, 0, 3), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), &window);
        
        particleManager = new ParticleManager();
        particleRenderer = new ParticleRenderer(particleManager, &loader); 
        std::vector<std::string> filenames;
        filenames.push_back("models/particleStar.png");
        filenames.push_back("models/star1.png");
        filenames.push_back("models/star2.png");
        ParticleTexture *texture = loader.loadParticleTexture(filenames);
        emitter = new ParticleEmitter(particleManager, texture, glm::vec3(-2, 2, 0), 250, glm::vec3(1.5, 3, 1.5), -5, 3, false);

        Entity *teapot = loader.loadEntity("models/teapot/teapot20segUT.obj", teapot1);
        teapot->position = glm::vec3(0, 0, 0);
        teapot->rotation = glm::vec3(0, 0, 0);
        teapot->scale = glm::vec3(1, 1, 1);
        teapot->ambientStrength = 0.2f;
        teapot->specularStrength = 0.5f;
        renderer->addEntity(teapot);

        teapot = loader.loadEntity("models/ground/ground.obj", teapot2);
        teapot->position = glm::vec3(0, -1, 0);
        teapot->rotation = glm::vec3(0, 3.14f / 2, 0);
        teapot->scale = glm::vec3(1, 1, 1);
        teapot->ambientStrength = 0.2f;
        teapot->specularStrength = 0.5f;
        renderer->addEntity(teapot);

        teapot = loader.loadEntity("models/ground/ground.obj", teapot2);
        teapot->position = glm::vec3(3, 0, 0);
        teapot->rotation = glm::vec3(0, 0, 3.14f / 2);
        teapot->scale = glm::vec3(1, 1, 1);
        teapot->ambientStrength = 0.2f;
        teapot->specularStrength = 0.5f;
        renderer->addEntity(teapot);

        teapot = loader.loadEntity("models/nanosuit/nanosuit.obj", teapot2);
        teapot->position = glm::vec3(0, 0, 0);
        // teapot->rotation = glm::vec3(0, 3.14f / 2, 0);
        teapot->scale = glm::vec3(1, 1, 1);
        teapot->ambientStrength = 0.2f;
        teapot->specularStrength = 0.5f;
        renderer->addEntity(teapot);

        loader.loadEntity("models/quad/quad.obj", quadEntity);

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

        Animation<Entity> *animation = animator.createTriggeredAnimation<Entity>([&]()
                                                                                 { return keyboard->isKeyPressed(GLFW_KEY_P); });
        Animation<Entity> *teapotA = animator.createPeriodicAnimation<Entity>(3, 3);
        Animation<Entity> *subComponent = animator.createTriggeredAnimation<Entity>([&]()
                                                                                    { return keyboard->isKeyPressed(GLFW_KEY_P); });
        animation->addKeyFrame(KeyFrame<Entity>());
        animation->addKeyFrame(KeyFrame<Entity>(1.5, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
        subComponent->addKeyFrame(KeyFrame<Entity>());
        subComponent->addKeyFrame(KeyFrame<Entity>(1.5, glm::vec3(0, 0, 0), glm::vec3(0, 3.14f, 0), glm::vec3(1, 1, 1)));
        teapotA->addKeyFrame(KeyFrame<Entity>());
        teapotA->addKeyFrame(KeyFrame<Entity>(1.5, glm::vec3(-5, 0, 0), glm::vec3(0, 3.14f, 0), glm::vec3(1, 1, 1)));
        teapotA->addKeyFrame(KeyFrame<Entity>(2.5, glm::vec3(-5, 3, 0), glm::vec3(0, 2 * 3.14f, 0), glm::vec3(1, 1, 1)));
        loader.getEntity(this->teapot1)->attachMainComponentAnimation(teapotA);
        loader.getEntity(teapot2)->attachMainComponentAnimation(animation);
        loader.getEntity(teapot2)->attachSubComponentAnimation(subComponent, "Helmet_Helmet");
        loader.getEntity(teapot2)->attachSubComponentAnimation(subComponent, "Visor_Glass");
        renderer->enableSkyBox = true;
        renderer->fogDensity = 0.02f;
        particleRenderer->init();
    }

    void Application::update(double delta)
    {
        if (keyboard->isKeyPressed(GLFW_KEY_W))
            camera->move(gps::MOVE_FORWARD, delta * cameraSpeed);

        if (keyboard->isKeyPressed(GLFW_KEY_S))
            camera->move(gps::MOVE_BACKWARD, delta * cameraSpeed);

        if (keyboard->isKeyPressed(GLFW_KEY_A))
            camera->move(gps::MOVE_LEFT, delta * cameraSpeed);

        if (keyboard->isKeyPressed(GLFW_KEY_D))
            camera->move(gps::MOVE_RIGHT, delta * cameraSpeed);

        if (keyboard->getDisplayMode() == 1)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        // if (keyboard->isKeyPressed(GLFW_KEY_F))
            // particleManager->addParticle(Particle(glm::vec3(0, 3, 0), glm::vec3(rand() % 10, rand() % 10, rand() % 10), -10.0f, 3, 0, 0.1));

        animator.updateAnimations(delta);
        particleManager->update(delta);
        emitter->emitParticles(delta);

        static float mouseSpeed = 0.02f;
        double xpos, ypos;
        glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
        WindowDimensions dim = window.getWindowDimensions();
        pitch += delta * mouseSpeed * float(dim.width / 2 - xpos);
        yaw += delta * mouseSpeed * float(dim.height / 2 - ypos);
        camera->rotate(pitch, yaw);
        glfwSetCursorPos(window.getWindow(), dim.width / 2, dim.height / 2);
    }

    void Application::render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (keyboard->isKeyPressed(GLFW_KEY_M))
        {
            renderer->displayDirectionalAndSpotLightShadowMap(false, loader.getEntity(quadEntity), 0);
        }
        else
        {
            if (keyboard->getDisplayMode() == 2)
                renderer->renderEntitiesWithFaces(camera);
            else {
                renderer->renderEntities(camera);
                particleRenderer->render(camera);
            }
        }
    }

    void Application::run()
    {
        init();
        glCheckError();
        std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();
        while (!glfwWindowShouldClose(window.getWindow()))
        {
            std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
            double delta = std::chrono::duration_cast<std::chrono::duration<double>>(now - last).count();
            last = now;
            update(delta);
            render();

            glfwPollEvents();
            glfwSwapBuffers(window.getWindow());

            glCheckError();
        }
        cleanup();
    }

    void Application::cleanup()
    {
        window.deleteWindow();
    }

    void Application::windowResizeCallback(GLFWwindow *window, int width, int height)
    {
        fprintf(stdout, "Window resized! New width: %d , and height: %d\n", width, height);
    }
}