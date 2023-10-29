#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <stdexcept>

#include "PXE/pe_init.hpp"
#include "PXE/pe_window.hpp"
#include "PXE/__pe_win32.hpp"

#if defined(_WIN32) || defined(_WIN64)
    #define hot likely
#endif

namespace px {
    
    Window::Window(int width, int height, const char* title)
    : height(height), width(width), title(title) {
        if (!glfwInit()) throw std::runtime_error(std::string(std::to_string(__LINE__)+"| "+__FILE__+" error: `glfwInit()` function returns `GLFW_FALSE`.").c_str());
        this->window  = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (this->window == nullptr) {
            glfwTerminate();
            throw std::runtime_error(std::string(std::to_string(__LINE__)+"| "+__FILE__+" error: unable to create a window.").c_str());
        }
        glfwMakeContextCurrent(this->window);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        glfwSetWindowSizeCallback(window, wscall);
        glfwSetFramebufferSizeCallback(window, fbscall);
        glfwSetCursorPosCallback(window, Mouse::cpcall);
        glfwSetCursorEnterCallback(window, Mouse::cecall);
        glfwSetMouseButtonCallback(window, Mouse::mbcall);
        glfwSetKeyCallback(window, Keyboard::kbcall);

        #ifdef GLEW_VERSION
            if (glewInit() != GLEW_OK) {
                glfwDestroyWindow(window);
                glfwTerminate();
                return;
            }           
        #else
            static_assert(true, "error: unable to initialize GLEW.");
        #endif
        this->setVsync(false);
        glEnable(GL_DEPTH_TEST);
        glfwSetWindowUserPointer(window, this);

    }

    Window::Window(const char *title, bool windowed)
    : width(1), height(1), title(title) {

        if (!glfwInit()) throw std::runtime_error(std::string(std::to_string(__LINE__)+"| "+__FILE__+" error: `glfwInit()` function returns `GLFW_FALSE`.").c_str());

        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_DECORATED, GL_FALSE);

        this->window  = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (this->window == nullptr) {
            glfwTerminate();
            throw std::runtime_error(std::string(std::to_string(__LINE__)+"| "+__FILE__+" error: unable to create a window.").c_str());
        }

        glfwSetWindowPos(window, 0, 0);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        if (monitor == nullptr) throw std::runtime_error(std::string(std::to_string(__LINE__)+"| "+__FILE__+" error: unable to get primary monitor.").c_str());
        const GLFWvidmode* vmode = glfwGetVideoMode(monitor);
        if (vmode == nullptr) throw std::runtime_error(std::string(std::to_string(__LINE__)+"| "+__FILE__+" error: unable to get video mode.").c_str());
        *(int*)&width = vmode->width, *(int*)&height = vmode->height;
        if (windowed) glfwSetWindowSize(this->window, width, height);
        else glfwSetWindowMonitor(this->window, monitor, 0, 0, width, height, vmode->refreshRate);
        glfwMakeContextCurrent(this->window);
        glViewport(0, 0, width, height);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetCursorPosCallback(window, Mouse::cpcall);
        glfwSetCursorEnterCallback(window, Mouse::cecall);
        glfwSetMouseButtonCallback(window, Mouse::mbcall);
        glfwSetKeyCallback(window, Keyboard::kbcall);

        #ifdef GLEW_VERSION
            if (glewInit() != GLEW_OK) {
                glfwDestroyWindow(window);
                glfwTerminate();
                return;
            }
        #else
            static_assert(true, "error: unable to initialize GLEW.");
        #endif
        this->setVsync(false);
        glEnable(GL_DEPTH_TEST);
        glfwSetWindowUserPointer(window, this);

    }

    Window::~Window() {
        glfwDestroyWindow(this->window);
        glfwTerminate();
    }

    void Window::run(RendererCallback callback)
    {

        #if defined(__linux__) || defined(__unix__)
        
        FILE* pipe  = popen("gsettings get org.gnome.desktop.peripherals.touchpad disable-while-typing", "r");
        char val[6];

        if (pipe && fgets(val, 6, pipe) && !memcmp(val, "true", 4)) 
        {
            system("gsettings set org.gnome.desktop.peripherals.touchpad disable-while-typing false");
        }

        pclose(pipe);

        #endif

        try {
            double lastFrameTime, deltaTime, currentFrameTime;
            lastFrameTime = deltaTime = .0;
            
            #pragma omp parallel
            while (!this->shouldClose()) [[hot]] {

                #pragma omp atomic update
                {
                currentFrameTime = glfwGetTime();
                deltaTime = currentFrameTime - lastFrameTime;
                lastFrameTime = currentFrameTime;
                }
                
                this->clear();
                // Draw call
                [[hot]] callback(deltaTime);

                this->swapBuffers();
                this->pollEvents();
                
            }
            
            glfwTerminate();

        } catch (std::exception& e) {}
    
    
        #if defined(__linux__) || defined(__unix__)

        if (!memcmp(val, "false", 5)) return;

        system("gsettings set org.gnome.desktop.peripherals.touchpad disable-while-typing true");

        #endif
    }

    void Window::setVsync(bool value) const { glfwSwapInterval(value); }
    void Window::hideWindow() const { glfwHideWindow(window); }
    void Window::showWindow() const { glfwShowWindow(window); }
    void Window::swapBuffers() const { glfwSwapBuffers(this->window); }
    void Window::pollEvents() const { glfwPollEvents(); }
    void Window::clear() const { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
    int Window::shouldClose() const { return glfwWindowShouldClose(this->window); }
    void Window::closeWindow() const { glfwSetWindowShouldClose(window, GLFW_TRUE); }


    void Window::wscall(GLFWwindow* win, int w, int h) {
        px::Window* uw = static_cast<Window*>(glfwGetWindowUserPointer(win));
        glViewport(0, 0, uw->width, uw->height);
    }
    void Window::fbscall(GLFWwindow* win, int width, int height) {
        px::Window* uw = static_cast<px::Window*>(glfwGetWindowUserPointer(win));
        *(int*)&uw->width = width, *(int*)&uw->height = height;
    }

} // namespace px

#ifdef hot
    #undef hot
#endif