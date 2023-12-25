#include <engine/Window.hpp>

namespace gps
{

    void Window::createWindow(bool fullScreen, int width, int height, const char *title)
    {
        if (!glfwInit())
            throw std::runtime_error("Could not start GLFW3!");

        // window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // window scaling for HiDPI displays
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

        // for sRBG framebuffer
        glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

        // for antialising
        glfwWindowHint(GLFW_SAMPLES, 4);

        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        if (fullScreen)
            this->window = glfwCreateWindow(mode->width, mode->height, title, glfwGetPrimaryMonitor(), NULL);
        else  
            this->window = glfwCreateWindow(width, height, title, NULL, NULL);

        if (!this->window)
            throw std::runtime_error("Could not create GLFW3 window!");

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        if (fullScreen)
            glfwSetCursorPos(window, mode->width / 2, mode->height / 2);
        else
            glfwSetCursorPos(window, width / 2, height / 2);

        glfwMakeContextCurrent(window);

        glfwSwapInterval(1);

#if not defined(__APPLE__)
        // start GLEW extension handler
        glewExperimental = GL_TRUE;
        glewInit();
#endif

        // get version info
        const GLubyte *renderer = glGetString(GL_RENDERER); // get renderer string
        const GLubyte *version = glGetString(GL_VERSION);   // version as a string
        std::cout << "Renderer: " << renderer << std::endl;
        std::cout << "OpenGL version: " << version << std::endl;

        // for RETINA display
        glfwGetFramebufferSize(window, &this->dimensions.width, &this->dimensions.height);
    }

    void Window::deleteWindow()
    {
        if (window)
            glfwDestroyWindow(window);
        // close GL context and any other GLFW resources
        glfwTerminate();
    }

    GLFWwindow *Window::getWindow()
    {
        return this->window;
    }

    WindowDimensions Window::getWindowDimensions()
    {
        return this->dimensions;
    }

    void Window::setWindowDimensions(const WindowDimensions &dimensions)
    {
        this->dimensions = dimensions;
    }

    void Window::setResizeCallback(GLFWwindowsizefun callback)
    {
        glfwSetWindowSizeCallback(window, callback);
    }

    void Window::setKeyCallback(GLFWkeyfun callback)
    {
        glfwSetKeyCallback(window, callback);
    }

    void Window::setMouseCallback(GLFWcursorposfun callback)
    {
        glfwSetCursorPosCallback(window, callback);
    }
}
