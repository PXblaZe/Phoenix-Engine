
#include <iostream>


#include "PXE/pe_window.hpp"

namespace px {
    
    Window::Window(int width, int height, const char* title)
    : height(height), width(width), title(title) {
        if (!glfwInit()) std::__throw_runtime_error(std::string(std::to_string(__LINE__)+"| "+__FILE__+" error: `glfwInit()` function returns `GLFW_FALSE`.").c_str());
        this->window  = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (this->window == nullptr) {
            glfwTerminate();
            std::__throw_runtime_error(std::string(std::to_string(__LINE__)+"| "+__FILE__+" error: unable to create a window.").c_str());
        }
        glfwMakeContextCurrent(this->window);
        #ifdef GLEW_VERSION
            glewInit();
        #endif
    }

    Window::Window(const char *title, bool windowed)
    : width(1), height(1), title(title) {

        if (!glfwInit()) std::__throw_runtime_error(std::string(std::to_string(__LINE__)+"| "+__FILE__+" error: `glfwInit()` function returns `GLFW_FALSE`.").c_str());

        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_DECORATED, GL_FALSE);

        this->window  = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (this->window == nullptr) {
            glfwTerminate();
            std::__throw_runtime_error(std::string(std::to_string(__LINE__)+"| "+__FILE__+" error: unable to create a window.").c_str());
        }

        glfwSetWindowPos(window, 0, 0);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        if (monitor == nullptr) std::__throw_runtime_error(std::string(std::to_string(__LINE__)+"| "+__FILE__+" error: unable to get primary monitor.").c_str());
        const GLFWvidmode* vmode = glfwGetVideoMode(monitor);
        if (vmode == nullptr) std::__throw_runtime_error(std::string(std::to_string(__LINE__)+"| "+__FILE__+" error: unable to get video mode.").c_str());
        *(int*)&width = vmode->width, *(int*)&height = vmode->height;
        if (windowed) glfwSetWindowSize(this->window, width, height);
        else glfwSetWindowMonitor(this->window, monitor, 0, 0, width, height, vmode->refreshRate);
        glfwMakeContextCurrent(this->window);
        #ifdef GLEW_VERSION
            glewInit();
            glEnable(GL_DEPTH_TEST);
        #endif
    }

    Window::~Window() {
        glfwDestroyWindow(this->window);
        glfwTerminate();
    }

    void Window::hideWindow() const {
        glfwHideWindow(window);
    }

    void Window::showWindow() const {
        glfwShowWindow(window);
    }

} // namespace px
