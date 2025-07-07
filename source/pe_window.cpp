#include <cstdio>
#include <ios>
#include <string>
#include <iostream>
// #include <stdio.h>
// #include <cstdlib>
// #include <string.h>
// #include <stdlib.h>
#include <stdexcept>
#ifdef __linux__
#include <gio/gio.h>
#include <gio/gsettings.h>
#endif

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

        glfwSetCursorPos(window, 0, 0);
        
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

        glfwSetWindowPos(window, 0, 0);

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

        glfwSetCursorPos(window, 0, 0);
        
    }

    Window::~Window() {
        glfwDestroyWindow(this->window);
        glfwTerminate();
    }

    void Window::run(RendererCallback callback)
    {
        double xp, yp;
        
        #if defined(__linux__) || defined(__unix__)
        GSettings* settings = g_settings_new("org.gnome.desktop.peripherals.touchpad");
        gboolean dwt_flag = g_settings_get_boolean(settings, "disable-while-typing");
        std::cout << "dwt_flag = " << dwt_flag << std::endl;
        if (dwt_flag == TRUE) 
        {
            g_settings_set_boolean(settings, "disable-while-typing", false);
            std::cout << "dwt: " << g_settings_get_boolean(settings, "disable-while-typing") << std::endl;
        }


        #endif

        try {
            double lastFrameTime, deltaTime, currentFrameTime, one_secTime, frameCount, fps;
            lastFrameTime = deltaTime = one_secTime = frameCount = .0;
                
            this->pollEvents();
            #pragma omp parallel
            while (!this->shouldClose()) [[hot]] {

                glfwGetCursorPos(window, &xp, &yp);
                // printf("Curpos: {%f, %f}\n", xp, yp);

                #pragma omp atomic update
                {
                frameCount++;
                currentFrameTime = glfwGetTime();
                deltaTime = currentFrameTime - lastFrameTime;
                lastFrameTime = currentFrameTime;
                if (currentFrameTime - one_secTime >= 1.0){
                    fps = frameCount, frameCount = .0;
                    one_secTime = currentFrameTime;
                }
                }
                
                this->clear();
                // Draw call
                [[hot]] callback(deltaTime, fps);

                this->swapBuffers();
                this->pollEvents();
                
            }
            
            glfwTerminate();

        } catch (std::exception& e) {}
    
    
        #if defined(__linux__) || defined(__unix__)

        if (dwt_flag == TRUE) {
            // system("gsettings set org.gnome.desktop.peripherals.touchpad disable-while-typing true");
            g_settings_set_boolean(settings, "disable-while-typing", true);
            std::cout << "dwt1: " << g_settings_get_boolean(settings, "disable-while-typing") << std::endl;
        }
        g_object_unref(settings);

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
    void Window::setTitle(const char* title) const { 
        glfwSetWindowTitle(window, title);
        *(decltype(&this->title)*)(&this->title) = *(decltype(&title)*)(&title); 
    }


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
