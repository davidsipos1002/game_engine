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
#include <particles/Particle.hpp>
#include <particles/ParticleManager.hpp>
#include <particles/ParticleRenderer.hpp>
#include <particles/ParticleEmitter.hpp>
#include <particles/ParticleTexture.hpp>
#include <cmath>
#include <functional>
namespace gps
{
    class Application
    {
    protected:
        Window window;
        Keyboard *keyboard;
        Mouse *mouse;
        Loader *loader;
        Renderer *renderer;
        Animator *animator;
        ParticleManager *particleManager;
        ParticleRenderer *particleRenderer;
        Camera *camera;

        GLfloat cameraSpeed;
        float mouseSensitivity;

    public:
        Application(bool fullScreen, int width, int height, const std::string &title);
        virtual ~Application(){};
        void run();

    private:
        static void windowResizeCallback(GLFWwindow *window, int width, int height);
        virtual void init() = 0;
        virtual void update(double delta) = 0;
        virtual bool doCustomRender();
        virtual void render();
        virtual void cleanup() = 0;
        void __init();
        void __update(double delta);
        void updateCamera(double delta);
        void __render();
        void __cleanup();
    };
}