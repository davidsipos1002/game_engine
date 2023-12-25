#include <io/Mouse.hpp>
#include <iostream>

namespace gps
{
    Mouse *Mouse::instance = nullptr;
    double Mouse::xpos = 0;
    double Mouse::ypos = 0;
    double Mouse::prev_xpos = 0;
    double Mouse::prev_ypos = 0;
    bool Mouse::positionChanged = false;

    Mouse::Mouse(Window &window)
    {
        window.setMouseCallback(mouseCallback);
        glfwGetCursorPos(window.getWindow(), &xpos, &ypos);
        prev_xpos = xpos;
        prev_ypos = ypos;
    }

    void Mouse::mouseCallback(GLFWwindow *window, double xpos, double ypos)
    {
        prev_xpos = Mouse::xpos;
        prev_ypos = Mouse::ypos;
        Mouse::xpos = xpos;
        Mouse::ypos = ypos;
    }

    Mouse *Mouse::getInstance(Window &window)
    {
        if (instance == nullptr)
            instance = new Mouse(window);
        return instance;
    }

    void Mouse::freeInstance()
    {
        delete instance;
        instance = nullptr;
    }

    void Mouse::getPosition(double x, double y)
    {
        x = xpos;
        y = ypos;
    }

    void Mouse::getPreviousPosition(double x, double y)
    {
        x = prev_xpos;
        y = prev_ypos;
    }
}
