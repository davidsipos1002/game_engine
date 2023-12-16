#pragma once

#include <GeneralIncludes.hpp>
#include <Window.h>
#include <Keyboard.hpp>
#include <Mouse.hpp>
#include <Model3D.hpp>
#include <Shader.hpp>
#include <Camera.hpp>

namespace gps
{
    class Application
    {
    private:
        Window window;
        Keyboard *keyboard;
        Mouse *mouse;
        Model3D teapot;
        Shader shader;
        // matrices
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat3 normalMatrix;

        // light parameters
        glm::vec3 lightDir;
        glm::vec3 lightColor;

        // shader uniform locations
        GLint modelLoc;
        GLint viewLoc;
        GLint projectionLoc;
        GLint normalMatrixLoc;
        GLint lightDirLoc;
        GLint lightColorLoc;
        // models
        GLfloat angle;
        // camera
        Camera camera = Camera(
            glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, -10.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));

        GLfloat cameraSpeed = 0.1f;

        float pitch = -3.14f;
        float yaw;

    public:
        Application();
        ~Application();
        void run();

    private:
        static void windowResizeCallback(GLFWwindow *window, int width, int height);
        void initUniforms();
        void init();
        void update(double delta);
        void render();
        void cleanup();
    };
}