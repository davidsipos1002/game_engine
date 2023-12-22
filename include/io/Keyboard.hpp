#pragma once

#include <engine/GeneralIncludes.hpp>
#include <graphics/Window.h>

namespace gps
{
    class Keyboard
    {
    private:
        static Keyboard *instance;
        static bool pressedKeys[1024];

        Keyboard(Window &window);
        static void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

    public:

        Keyboard(Keyboard &other) = delete;
        void operator=(const Keyboard &other) = delete;
        
        Keyboard(Keyboard &&other) = delete;
        void operator=(Keyboard &&other) = delete;

        static Keyboard *getInstance(Window &window);
        static void freeInstance();
        bool isKeyPressed(int key);
    };
}