#pragma once

#include "PXE/pe_init.hpp"
namespace px {

    class Window {

        GLFWwindow* window;

    public:

        template<typename... ARGS>
        using RendererCallback =  void (*)(ARGS...);

        const char* const title;
        const int height, width;
        Window(int width, int height, const char* title);
        Window(const char* title, bool windowed = true);
        ~Window();

        template<typename... Args>
        void run(Window::RendererCallback<Args...> callback, Args... params) const {
            while (!glfwWindowShouldClose(this->window)) {
                #pragma omp parallel
                {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Draw call
                #pragma omp critical
                {
                callback(params...);
                }
                }
                glfwSwapBuffers(this->window);
                glfwPollEvents();
            }
        }
        void hideWindow() const;
        void showWindow() const;
        Window(const Window&) = delete;
        Window operator=(const Window&) = delete;
    };

}