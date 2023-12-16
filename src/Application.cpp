#include <Application.hpp>
#include <ErrorCheck.hpp>

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

        // Init uniforms  
        initUniforms();
    }

    void Application::initUniforms()
    {
        shader.useShaderProgram();

        // create model matrix for teapot
        model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelLoc = glGetUniformLocation(shader.shaderProgram, "model");

        // get view matrix for current camera
        view = camera.getViewMatrix();
        viewLoc = glGetUniformLocation(shader.shaderProgram, "view");
        // send view matrix to shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        normalMatrixLoc = glGetUniformLocation(shader.shaderProgram, "normalMatrix");

        // create projection matrix
        projection = glm::perspective(glm::radians(45.0f),
                                      (float)window.getWindowDimensions().width / (float)window.getWindowDimensions().height,
                                      0.1f, 20.0f);
        projectionLoc = glGetUniformLocation(shader.shaderProgram, "projection");
        // send projection matrix to shader
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // set the light direction (direction towards the light)
        lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
        lightDirLoc = glGetUniformLocation(shader.shaderProgram, "lightDir");
        // send light dir to shader
        glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

        // set light color
        lightColor = glm::vec3(1.0f, 1.0f, 1.0f); // white light
        lightColorLoc = glGetUniformLocation(shader.shaderProgram, "lightColor");
        // send light color to shader
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    }

    void Application::update(double delta)
    {
        if (keyboard->isKeyPressed(GLFW_KEY_W))
        {
            camera.move(gps::MOVE_FORWARD, cameraSpeed);
            // update view matrix
            view = camera.getViewMatrix();
            shader.useShaderProgram();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            // compute normal matrix for teapot
            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        }

        if (keyboard->isKeyPressed(GLFW_KEY_S))
        {
            camera.move(gps::MOVE_BACKWARD, cameraSpeed);
            // update view matrix
            view = camera.getViewMatrix();
            shader.useShaderProgram();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            // compute normal matrix for teapot
            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        }

        if (keyboard->isKeyPressed(GLFW_KEY_A))
        {
            camera.move(gps::MOVE_LEFT, cameraSpeed);
            // update view matrix
            view = camera.getViewMatrix();
            shader.useShaderProgram();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            // compute normal matrix for teapot
            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        }

        if (keyboard->isKeyPressed(GLFW_KEY_D))
        {
            camera.move(gps::MOVE_RIGHT, cameraSpeed);
            // update view matrix
            view = camera.getViewMatrix();
            shader.useShaderProgram();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            // compute normal matrix for teapot
            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        }

        if (keyboard->isKeyPressed(GLFW_KEY_Q))
        {
            angle -= 1.0f;
            // update model matrix for teapot
            model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
            // update normal matrix for teapot
            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        }

        if (keyboard->isKeyPressed(GLFW_KEY_E))
        {
            angle += 1.0f;
            // update model matrix for teapot
            model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
            // update normal matrix for teapot
            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        }

        static float mouseSpeed = 0.0005f;
        double xpos, ypos;
        glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
        WindowDimensions dim = window.getWindowDimensions();
        pitch += mouseSpeed * float(dim.width / 2 - xpos);
        yaw += mouseSpeed * float(dim.height / 2 - ypos);
        camera.rotate(pitch, yaw);
        glfwSetCursorPos(window.getWindow(), dim.width / 2, dim.height / 2);

        view = camera.getViewMatrix();
        shader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    }

    void Application::render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // select active shader program
        shader.useShaderProgram();

        // send teapot model matrix data to shader
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // send teapot normal matrix data to shader
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        // draw teapot
        teapot.Draw(shader);
    }

    void Application::run()
    {
        init();
        glCheckError();
        while (!glfwWindowShouldClose(window.getWindow()))
        {
            update(0);
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