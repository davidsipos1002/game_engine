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
    }

    void Application::init()
    {
        window.Create(1024, 768, "OpenGL Project");
        window.setResizeCallback(windowResizeCallback);
        keyboard = Keyboard::getInstance(window);
        mouse = Mouse::getInstance(window);

        renderer = new Renderer(&window);
        shadow = new ShadowMap(window.getWindowDimensions().width, window.getWindowDimensions().height);

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

        teapot = loader.loadEntity("models/nanosuit/nanosuit.obj", teapot2);
        teapot->position = glm::vec3(0, 0, 0);
        // teapot->rotation = glm::vec3(0, 3.14f / 2, 0);
        teapot->scale = glm::vec3(1, 1, 1);
        teapot->ambientStrength = 0.2f;
        teapot->specularStrength = 0.5f;
        renderer->addEntity(teapot);

        loader.loadShader("shaders/entityNormal.vert", "shaders/entityNormal.frag", shader);
        loader.loadShader("shaders/directionalShadow.vert", "shaders/directionalShadow.frag", dirShadowMap);
        loader.loadShader("shaders/shadowMap.vert", "shaders/shadowMap.frag", quad);
        loader.loadEntity("models/quad/quad.obj", quadEntity);

        renderer->directionalLights[0].intensity = 0.2f;
        renderer->directionalLights[0].lightColor = glm::vec3(1, 1, 1);
        renderer->directionalLights[0].lightDirection = glm::vec3(0, 1, 1);
        renderer->directionalLights[0].calculateLightSpaceMatrix();

        renderer->directionalLights[1].intensity = 0.0f;
        renderer->directionalLights[1].lightColor = glm::vec3(0, 1, 0);
        renderer->directionalLights[1].lightDirection = glm::vec3(0, -1, -1);

        renderer->pointLights[0].intensity = 0.3f;
        renderer->pointLights[0].lightColor = glm::vec3(0, 0, 1);
        renderer->pointLights[0].lightPosition = glm::vec3(-3, 2, 0);

        renderer->pointLights[1].intensity = 0.2f;
        renderer->pointLights[1].lightColor = glm::vec3(1, 1, 0);
        renderer->pointLights[1].lightPosition = glm::vec3(3, 2, 0);

        renderer->spotLights[0].intensity = 2.0f;
        renderer->spotLights[0].lightPosition = glm::vec3(0.75, 3, 0);
        renderer->spotLights[0].lightDirection = glm::vec3(0, -1, 0);
        renderer->spotLights[0].lightColor = glm::vec3(1, 1, 1);
        renderer->spotLights[0].cutoff = cos(3.14f / 10);

        projection = glm::perspective(glm::radians(45.0f),
                                      (float)window.getWindowDimensions().width / (float)window.getWindowDimensions().height,
                                      0.1f, 20.0f);
        
        Animation<Entity> *animation = animator.createTriggeredAnimation<Entity>([&] () {return keyboard->isKeyPressed(GLFW_KEY_P);});
        Animation<Entity> *teapotA = animator.createPeriodicAnimation<Entity>(3, 3);
        Animation<Entity> *subComponent = animator.createTriggeredAnimation<Entity>([&] () {return keyboard->isKeyPressed(GLFW_KEY_P);});
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
    }

    void Application::update(double delta)
    {
        if (keyboard->isKeyPressed(GLFW_KEY_W))
            camera.move(gps::MOVE_FORWARD, delta * cameraSpeed);

        if (keyboard->isKeyPressed(GLFW_KEY_S))
            camera.move(gps::MOVE_BACKWARD, delta * cameraSpeed);

        if (keyboard->isKeyPressed(GLFW_KEY_A))
            camera.move(gps::MOVE_LEFT, delta * cameraSpeed);

        if (keyboard->isKeyPressed(GLFW_KEY_D))
            camera.move(gps::MOVE_RIGHT, delta * cameraSpeed);

        if (keyboard->isKeyPressed(GLFW_KEY_Q))
        {
            angle -= 1.0f;
            loader.getEntity(teapot1)->rotation.y = glm::radians(angle);
        }

        if (keyboard->isKeyPressed(GLFW_KEY_E))
        {
            angle += 1.0f;
            loader.getEntity(teapot1)->rotation.y = glm::radians(angle);
        }

        animator.updateAnimations(delta);
            
        static float mouseSpeed = 0.02f;
        double xpos, ypos;
        glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
        WindowDimensions dim = window.getWindowDimensions();
        pitch += delta * mouseSpeed * float(dim.width / 2 - xpos);
        yaw += delta * mouseSpeed * float(dim.height / 2 - ypos);
        camera.rotate(pitch, yaw);
        glfwSetCursorPos(window.getWindow(), dim.width / 2, dim.height / 2);
    }

    void Application::render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (keyboard->isKeyPressed(GLFW_KEY_M)) {
        renderer->renderShadowMap(loader.getEntity(quadEntity), loader.getShader(quad), shadow);
        } else {
        renderer->renderEntities(&camera, loader.getShader(shader), projection, shadow);
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
        window.Delete();
    }

    void Application::windowResizeCallback(GLFWwindow *window, int width, int height)
    {
        fprintf(stdout, "Window resized! New width: %d , and height: %d\n", width, height);
    }
}