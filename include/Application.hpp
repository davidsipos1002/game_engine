#pragma once

#include <GeneralIncludes.hpp>
#include <Window.h>
#include <Keyboard.hpp>
#include <Mouse.hpp>
#include <Entity.hpp>
#include <Renderer.hpp>
#include <Shader.hpp>
#include <Camera.hpp>
#include <Loader.hpp>

namespace gps
{
    class Application
    {
    private:
        Window window;
        Keyboard *keyboard;
        Mouse *mouse;
        Loader loader;
        Renderer renderer;
        
        std::string teapot1;
        std::string teapot2;
        std::string shader;

        glm::mat4 projection;

        GLfloat angle;

        Camera camera = Camera(
            glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, -10.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));

        GLfloat cameraSpeed = 0.8f;

        float pitch = -3.14f;
        float yaw = 0;

    public:
        Application();
        ~Application();
        void run();

    private:
        static void windowResizeCallback(GLFWwindow *window, int width, int height);
        void init();
        void update(double delta);
        void render();
        void cleanup();
    };
}