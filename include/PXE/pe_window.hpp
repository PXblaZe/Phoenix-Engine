#pragma once

#include <PXE/pe_viewspace.hpp>

namespace px {

    class Window {

        typedef struct GLFWwindow GLFWwindow;

        GLFWwindow* window;
        Viewspace* camera;

        template<typename... ARGS>
        using RendererCallback =  void (*)(ARGS...);

        static void wscall(GLFWwindow* win, int w, int h);
        static void fbscall(GLFWwindow* win, int width, int height);


    public:

        const char* const title;
        const int height, width;
        Window(int width, int height, const char* title);
        Window(const char* title, bool windowed = true);
        ~Window();

        template<typename... Args>
        void run(Window::RendererCallback<Args...> callback, Args... params) const {
            while (!this->shouldClose()) {
                #pragma omp parallel
                {
                this->clear();

                // Draw call
                #pragma omp critical
                {
                callback(params...);
                }
                }
                this->swapBuffers();
                this->pollEvents();
            }
        }

        void hideWindow() const;
        void showWindow() const;
        void swapBuffers() const;
        void pollEvents() const;
        void clear() const;
        void setVsync(bool value) const;
        int shouldClose() const;

        Window(const Window&) = delete;
        Window operator=(const Window&) = delete;
    };

}