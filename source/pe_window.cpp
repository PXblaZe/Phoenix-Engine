#include <string>
#include <stdexcept>

#include "PXE/pe_init.hpp"
#include "PXE/pe_window.hpp"


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
        // glfwSetInputMode(window, GLFW_CURSOR, );
        
        glfwSetWindowSizeCallback(window, wscall);
        glfwSetFramebufferSizeCallback(window, fbscall);
        glfwSetCursorPosCallback(window, cpcall);
        glfwSetCursorEnterCallback(window, cecall);
        glfwSetMouseButtonCallback(window, mbcall);
        glfwSetKeyCallback(window, kbcall);

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

        glfwSetCursorPosCallback(window, cpcall);
        glfwSetCursorEnterCallback(window, cecall);
        glfwSetMouseButtonCallback(window, mbcall);
        glfwSetKeyCallback(window, kbcall);

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
        while (!this->shouldClose()) [[likely]] {
            
            #pragma omp parallel
            {
                
                this->clear();
                // Draw call
                #pragma omp critical
                {
                // read_mouse_inputs = !read_mouse_inputs;
                // read_keyboard_inputs = !read_keyboard_inputs;
                [[likely]] callback();
                }
                
            }
            this->swapBuffers();
            this->pollEvents();
        }

        glfwTerminate();
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

    void Window::cpcall(GLFWwindow* win, double x, double y) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
        if (!window->read_mouse_inputs) return;
        window->mouse_input.x = x, window->mouse_input.y = y;
    }

    void Window::cecall(GLFWwindow* win, int ent) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));return;
        window->mouse_input.isCursorEntered = ent ? true: false;
    }

    void Window::mbcall(GLFWwindow* win, int button, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
        if (!window->read_mouse_inputs) return;
        window->mouse_input.button = button, window->mouse_input.action = action, window->mouse_input.mods = mods;
    }

    void Window::kbcall(GLFWwindow* win, int key, int scancode, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
        if (!window->read_keyboard_inputs) return;
        window->keyboard_input.key = key;
        window->keyboard_input.scancode = scancode;
        window->keyboard_input.action = action;
        window->keyboard_input.mods = mods;
        window->keyboard_input.key_action_record[key] = action;
    }

} // namespace px
