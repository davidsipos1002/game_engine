#include <Application.hpp>
#include <ErrorCheck.hpp>
#include <chrono>

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
        // Create window
        window.Create(1024, 768, "OpenGL Project");
        window.setResizeCallback(windowResizeCallback);
        keyboard = Keyboard::getInstance(window);
        mouse = Mouse::getInstance(window);

        // Init OpenGL
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glViewport(0, 0, window.getWindowDimensions().width, window.getWindowDimensions().height);
        glEnable(GL_FRAMEBUFFER_SRGB);
        glEnable(GL_DEPTH_TEST); // enable depth-testing
        glDepthFunc(GL_LESS);    // depth-testing interprets a smaller value as "closer"
        glEnable(GL_CULL_FACE);  // cull face
        glCullFace(GL_BACK);     // cull back face
        glFrontFace(GL_CCW);     // GL_CCW for counter clock-wise

        // Init models

        Model3D *tea = new Model3D();
        tea->LoadModel("models/teapot/teapot20segUT.obj");
        teapot = new Entity(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), tea);
        // Init shaders
        shader.loadShader("shaders/basic.vert", "shaders/basic.frag");
        
        renderer.addEntity(teapot);
        renderer.directionalLight.intensity = 1.0f;
        renderer.directionalLight.lightColor = glm::vec3(1, 1, 1);
        renderer.directionalLight.lightDirection = glm::vec3(0, 1, 1);
        
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
            teapot->rotation.y = glm::radians(angle);
        }

        if (keyboard->isKeyPressed(GLFW_KEY_E))
        {
            angle += 1.0f;
            teapot->rotation.y = glm::radians(angle);
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
        
        renderer.renderEntities(&camera, &shader, projection);
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