
#include <iostream>


#include "PXE/pe_init.hpp"
#include "PXE/pe_window.hpp"


namespace px {

    void Window::wscall(GLFWwindow* win, int w, int h) {
        px::Window* uw = static_cast<Window*>(glfwGetWindowUserPointer(win));
        glViewport(0, 0, uw->width, uw->height);
    }
    void Window::fbscall(GLFWwindow* win, int width, int height) {
        px::Window* uw = static_cast<px::Window*>(glfwGetWindowUserPointer(win));
        *(int*)&uw->width = width, *(int*)&uw->height = height;
    }
    
    Window::Window(int width, int height, const char* title)
    : height(height), width(width), title(title) {
        if (!glfwInit()) std::__throw_runtime_error(std::string(std::to_string(__LINE__)+"| "+__FILE__+" error: `glfwInit()` function returns `GLFW_FALSE`.").c_str());
        this->window  = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (this->window == nullptr) {
            glfwTerminate();
            std::__throw_runtime_error(std::string(std::to_string(__LINE__)+"| "+__FILE__+" error: unable to create a window.").c_str());
        }
        glfwMakeContextCurrent(this->window);
        glfwSetWindowSizeCallback(window, wscall);
        glfwSetFramebufferSizeCallback(window, fbscall);
        #ifdef GLEW_VERSION
            if (glewInit() != GLEW_OK) {
                glfwDestroyWindow(window);
                glfwTerminate();
                return;
            }           
        #endif
        glEnable(GL_DEPTH_TEST);
        glfwSetWindowUserPointer(window, this);

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
        glViewport(0, 0, width, height);
        #ifdef GLEW_VERSION
            if (glewInit() != GLEW_OK) {
                glfwDestroyWindow(window);
                glfwTerminate();
                return;
            }
        #endif
        glEnable(GL_DEPTH_TEST);
        glfwSetWindowUserPointer(window, this);

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

    void Window::swapBuffers() const { glfwSwapBuffers(this->window); }
    void Window::pollEvents() const { glfwPollEvents(); }
    void Window::clear() const { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
    int Window::shouldClose() const { return glfwWindowShouldClose(this->window); }

} // namespace px
