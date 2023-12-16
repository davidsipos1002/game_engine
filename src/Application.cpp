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
        teapot.LoadModel("models/teapot/teapot20segUT.obj");

        // Init shaders
        shader.loadShader("shaders/basic.vert", "shaders/basic.frag");
        shader.addUniform("model");
        shader.addUniform("view");
        shader.addUniform("normalMatrix");
        shader.addUniform("projection");
        shader.addUniform("lightDir");
        shader.addUniform("lightColor");

        // Init uniforms  
        initUniforms();
    }

    void Application::initUniforms()
    {
        shader.useShaderProgram();

        // create model matrix for teapot
        model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));

        // create projection matrix
        projection = glm::perspective(glm::radians(45.0f),
                                      (float)window.getWindowDimensions().width / (float)window.getWindowDimensions().height,
                                      0.1f, 20.0f);
        // send projection matrix to shader
        glUniformMatrix4fv(shader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // set the light direction (direction towards the light)
        lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
        // send light dir to shader
        glUniform3fv(shader.getUniformLocation("lightDir"), 1, glm::value_ptr(lightDir));

        // set light color
        lightColor = glm::vec3(1.0f, 1.0f, 1.0f); // white light
        glUniform3fv(shader.getUniformLocation("lightColor"), 1, glm::value_ptr(lightColor));
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
            model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
        }

        if (keyboard->isKeyPressed(GLFW_KEY_E))
        {
            angle += 1.0f;
            model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
        }

        static float mouseSpeed = 0.02f;
        double xpos, ypos;
        glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
        WindowDimensions dim = window.getWindowDimensions();
        pitch += delta * mouseSpeed * float(dim.width / 2 - xpos);
        yaw += delta * mouseSpeed * float(dim.height / 2 - ypos);
        camera.rotate(pitch, yaw);
        glfwSetCursorPos(window.getWindow(), dim.width / 2, dim.height / 2);

        view = camera.getViewMatrix();
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        shader.useShaderProgram();
        glUniformMatrix4fv(shader.getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    }

    void Application::render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // select active shader program
        shader.useShaderProgram();

        // send teapot model matrix data to shader
        glUniformMatrix4fv(shader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

        // send teapot normal matrix data to shader
        glUniformMatrix3fv(shader.getUniformLocation("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

        // draw teapot
        teapot.Draw(shader);
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