#pragma once

#include <functional>

#include "PXE/pe_events.hpp"
#include "PXE/pe_viewspace.hpp"

namespace px {

    class Window {

        typedef struct GLFWwindow GLFWwindow;

        GLFWwindow* window;

        typedef std::function<void(double, double)> RendererCallback;

        static void wscall(GLFWwindow* win, int w, int h);
        static void fbscall(GLFWwindow* win, int width, int height);

    public:

        const char* const title;
        const int height, width;

        Mouse mouse_input;
        Keyboard keyboard_input;

        Window(int width, int height, const char* title);
        Window(const char* title, bool windowed = true);
        ~Window();

        void run(RendererCallback callback);

        void hideWindow() const;
        void showWindow() const;
        void swapBuffers() const;
        void pollEvents() const;
        void clear() const;
        void setVsync(bool value) const;
        int shouldClose() const;
        void closeWindow() const;
        void setTitle(const char* title) const;

        Window(const Window&) = delete;
        Window operator=(const Window&) = delete;

    };

}   
