#pragma once

#include <engine/GeneralIncludes.hpp>
#include <graphics/Window.h>
#include <io/Keyboard.hpp>
#include <io/Mouse.hpp>
#include <object/Entity.hpp>
#include <graphics/Renderer.hpp>
#include <graphics/Shader.hpp>
#include <object/Camera.hpp>
#include <io/Loader.hpp>
#include <animation/Animation.hpp>
#include <animation/Animator.hpp>

namespace gps
{
    class Application
    {
    private:
        Window window;
        Keyboard *keyboard;
        Mouse *mouse;
        Loader loader;
        Renderer *renderer;
        Animator animator;
        
        std::string teapot1;
        std::string teapot2;
        std::string shader;
        std::string dirShadowMap;
        std::string quad;
        std::string quadEntity;

        glm::mat4 projection;

        Camera camera = Camera(
            glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, -10.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));

        GLfloat cameraSpeed = 1.5f;

        float pitch = -3.14f;
        float yaw = 0;
        int displayMode = 0;

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