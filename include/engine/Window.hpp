#pragma once

#include <engine/GeneralIncludes.hpp>

#include <stdexcept>
#include <iostream>

struct WindowDimensions
{
    int width;
    int height;
};

namespace gps
{
    class Window
    {

    public:
        void createWindow(bool fullScreen, int width = 800, int height = 600, const char *title = "OpenGL Project");
        void deleteWindow();
        void setResizeCallback(GLFWwindowsizefun callback);
        void setKeyCallback(GLFWkeyfun callback);
        void setMouseCallback(GLFWcursorposfun callback);

        GLFWwindow *getWindow();
        WindowDimensions getWindowDimensions();
        void setWindowDimensions(const WindowDimensions &dimensions);

    private:
        WindowDimensions dimensions;
        GLFWwindow *window;
    };
}