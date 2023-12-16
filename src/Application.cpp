#include <Application.hpp>
#include <ErrorCheck.hpp>
#include <chrono>

#define UUID_SYSTEM_GENERATOR
#include <uuid.h>

namespace gps
{
    Application::Application()
    {
    }

    Application::~Application()
    {
    }

    void Application::init()
    {
        window.Create(1024, 768, "OpenGL Project");
        window.setResizeCallback(windowResizeCallback);
        keyboard = Keyboard::getInstance(window);
        mouse = Mouse::getInstance(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glViewport(0, 0, window.getWindowDimensions().width, window.getWindowDimensions().height);
        glEnable(GL_FRAMEBUFFER_SRGB);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        Entity *teapot = loader.loadEntity("models/teapot/teapot20segUT.obj", teapot1);
        teapot->position = glm::vec3(0, 0, 0);
        teapot->rotation = glm::vec3(0, 0, 0);
        teapot->scale = glm::vec3(1, 1, 1);
        teapot->ambientStrength = 0.2f;
        teapot->specularStrength = 0.5f;
        renderer.addEntity(teapot);
        
        teapot = loader.loadEntity("models/teapot/teapot20segUT.obj", teapot2);
        teapot->position = glm::vec3(1, 1, 0);
        teapot->rotation = glm::vec3(0, 3.14f / 2, 0);
        teapot->scale = glm::vec3(1, 1, 0.5);
        teapot->ambientStrength = 0.2f;
        teapot->specularStrength = 0.5f;
        renderer.addEntity(teapot);

        loader.loadShader("shaders/entityNormal.vert", "shaders/entityNormal.frag", shader);
        
        renderer.directionalLights[0].intensity = 0.0f;
        renderer.directionalLights[0].lightColor = glm::vec3(1, 1, 1);
        renderer.directionalLights[0].lightDirection = glm::vec3(0, 1, 1);
        
        renderer.directionalLights[1].intensity = 0.5f;
        renderer.directionalLights[1].lightColor = glm::vec3(0, 1, 0);
        renderer.directionalLights[1].lightDirection = glm::vec3(0, -1, -1);
        
        renderer.pointLights[0].intensity = 1.5f;
        renderer.pointLights[0].lightColor = glm::vec3(0, 0, 1);
        renderer.pointLights[0].lightPosition = glm::vec3(0, -2, 0);
        
        renderer.pointLights[1].intensity = 0.4f;
        renderer.pointLights[1].lightColor = glm::vec3(1, 1, 0);
        renderer.pointLights[1].lightPosition = glm::vec3(0, 10, 0);
        
        projection = glm::perspective(glm::radians(45.0f),
                                      (float)window.getWindowDimensions().width / (float)window.getWindowDimensions().height,
                                      0.1f, 20.0f);
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
        
        if (keyboard->isKeyPressed(GLFW_KEY_L)) {
            renderer.directionalLights[0].intensity -= 0.001f;
        }

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
        
        renderer.renderEntities(&camera, loader.getShader(shader), projection);
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