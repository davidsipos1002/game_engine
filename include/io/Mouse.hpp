#pragma once

#include <engine/GeneralIncludes.hpp>

#include <engine/Window.hpp>

namespace gps
{
    class Mouse
    {
    private:
        static Mouse *instance;
        static double xpos;
        static double ypos;
        static double prev_xpos;
        static double prev_ypos;
        static bool positionChanged;

        Mouse(Window &window);

        static void mouseCallback(GLFWwindow *window, double xpos, double ypos);

    public:

        Mouse(Mouse &other) = delete;
        void operator=(const Mouse &other) = delete;
        
        Mouse(Mouse &&other) = delete;
        void operator=(Mouse &&other) = delete;
        
        static Mouse *getInstance(Window &window);
        static void freeInstance();

        void getPosition(double x, double y);
        void getPreviousPosition(double x, double y);
    };
}