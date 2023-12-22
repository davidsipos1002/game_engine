#include <io/Keyboard.hpp>

namespace gps
{
    Keyboard *Keyboard::instance = nullptr;
    bool Keyboard::pressedKeys[1024] = {};

    Keyboard::Keyboard(Window &window)
    {
        window.setKeyCallback(this->keyboardCallback);
    }

    void Keyboard::keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        if (key >= 0 && key < 1024)
        {
            if (action == GLFW_PRESS)
                pressedKeys[key] = true;
            else if (action == GLFW_RELEASE)
                pressedKeys[key] = false;
        }
    }

    Keyboard *Keyboard::getInstance(Window &window)
    {
        if (instance == nullptr)
            instance = new Keyboard(window);
        return instance;
    }
    
    void Keyboard::freeInstance()
    {
        delete instance;
        instance = nullptr;
    } 
    
    bool Keyboard::isKeyPressed(int key) {
        if (0 <= key && key < 1024)
            return pressedKeys[key];
        return false;
    }

}