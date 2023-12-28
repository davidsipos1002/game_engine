#include <engine/Application.hpp>
#include <chrono>
#include <tuple>

namespace gps
{
    Application::Application(bool fullScreen, int width, int height, const std::string &title) : cameraSpeed(5.0f), mouseSensitivity(0.02f)
    {
        window.createWindow(fullScreen, width, height, title.c_str());
    }

    void Application::__init()
    {
        window.setResizeCallback(windowResizeCallback);
        keyboard = Keyboard::getInstance(window);
        mouse = Mouse::getInstance(window);
        loader = new Loader();
        renderer = new Renderer(&window, loader, &skyBox);
        camera = new Camera(glm::vec3(0, 0, 3), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), &window);
        particleManager = new ParticleManager();
        particleRenderer = new ParticleRenderer(particleManager, loader);
        particleRenderer->init();
        animator = new Animator();
        init();
    }

    void Application::__update(double delta)
    {
        if (keyboard->getDisplayMode() == 1)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        updateCamera(delta);
        animator->updateAnimations(delta);
        particleManager->update(delta);
        update(delta);
    }

    void Application::updateCamera(double delta)
    {
        static float pitch = -M_PI;
        static float yaw = 0;
        Animation<Camera> *animation = camera->getAnimation();
        WindowDimensions dim = window.getWindowDimensions();
        if (animation && animation->isRunning())
        {
            camera->move(static_cast<MOVE_DIRECTION>(animation->interpolatedKeyFrame.direction), delta * animation->interpolatedKeyFrame.speed);
            pitch = animation->interpolatedKeyFrame.pitch;
            yaw = animation->interpolatedKeyFrame.yaw;
            camera->rotate(pitch, yaw);
        }
        else
        {
            if (keyboard->isKeyPressed(GLFW_KEY_W))
                camera->move(gps::MOVE_FORWARD, delta * cameraSpeed);

            if (keyboard->isKeyPressed(GLFW_KEY_S))
                camera->move(gps::MOVE_BACKWARD, delta * cameraSpeed);

            if (keyboard->isKeyPressed(GLFW_KEY_A))
                camera->move(gps::MOVE_LEFT, delta * cameraSpeed);

            if (keyboard->isKeyPressed(GLFW_KEY_D))
                camera->move(gps::MOVE_RIGHT, delta * cameraSpeed);
            double xpos, ypos;
            glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
            pitch += delta * mouseSensitivity * float(dim.width / 2 - xpos);
            yaw += delta * mouseSensitivity * float(dim.height / 2 - ypos);
            camera->rotate(pitch, yaw);
        }
        glfwSetCursorPos(window.getWindow(), dim.width / 2, dim.height / 2);
    }

    void Application::__render()
    {
        if (doCustomRender())
            render();
        else
        {
            if (keyboard->getDisplayMode() == 2)
                renderer->renderEntitiesWithFaces(camera);
            else
            {
                renderer->render(camera);
                particleRenderer->render(camera);
            }
        }
    }

    void Application::render()
    {
    }

    bool Application::doCustomRender()
    {
        return false;
    }

    void Application::run()
    {
        __init();
        std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();
        while (!glfwWindowShouldClose(window.getWindow()))
        {
            std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
            double delta = std::chrono::duration_cast<std::chrono::duration<double>>(now - last).count();
            last = now;
            __update(delta);
            __render();
            glfwPollEvents();
            glfwSwapBuffers(window.getWindow());
        }
        __cleanup();
    }

    void Application::__cleanup()
    {
        cleanup();
        renderer->cleanup();
        window.deleteWindow();
        delete renderer;
        delete camera;
        delete particleRenderer;
        delete particleManager;
        delete animator;
        delete loader;
    }

    void Application::windowResizeCallback(GLFWwindow *window, int width, int height)
    {
        fprintf(stdout, "Window resized! New width: %d , and height: %d\n", width, height);
    }
}