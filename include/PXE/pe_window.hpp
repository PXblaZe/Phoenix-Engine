#pragma once

#include <GLFW/glfw3.h>

namespace px {

    class Window {

        GLFWwindow* window;
        int initWindow();

    public:

        const char* const title;
        const int height, width;
        Window(int width, int height, const char* title);
        Window(const char* title, bool windowed = true);
        ~Window();
        void run() const;
        Window(const Window&) = delete;
        Window operator=(const Window&) = delete;
    };

}