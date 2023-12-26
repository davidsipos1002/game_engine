#pragma once

#include <engine/GeneralIncludes.hpp>
#include <engine/Window.hpp>
#include <io/Keyboard.hpp>
#include <io/Mouse.hpp>
#include <objects/Entity.hpp>
#include <graphics/Renderer.hpp>
#include <graphics/Shader.hpp>
#include <objects/Camera.hpp>
#include <io/Loader.hpp>
#include <animation/Animation.hpp>
#include <animation/Animator.hpp>
#include <cmath>
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

        Camera *camera;

        GLfloat cameraSpeed = 5.0f;

        int displayMode = 0;
        float pitch = -M_PI;
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