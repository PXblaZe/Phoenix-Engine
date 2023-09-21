#pragma once

#include <functional>

#include "PXE/pe_viewspace.hpp"

namespace px {

    struct MouseEvent {
        bool isCursorEntered;
        double x, y;
        int button, action, mods;
    };

    struct KeyboardEvent {
        int key, scancode, action, mods;
        char key_action_record[349] = {};
    };

    class Window {

        typedef struct GLFWwindow GLFWwindow;

        GLFWwindow* window;

        typedef std::function<void()> RendererCallback;

        static void wscall(GLFWwindow* win, int w, int h);
        static void fbscall(GLFWwindow* win, int width, int height);
        static void cpcall(GLFWwindow* win, double x, double y);
        static void cecall(GLFWwindow* win, int ent);
        static void mbcall(GLFWwindow* win, int button, int action, int mods);
        static void kbcall(GLFWwindow* win, int key, int scancode, int action, int mods);

    public:

        const char* const title;
        const int height, width;
        bool read_mouse_inputs = false, read_keyboard_inputs = false;

        MouseEvent mouse_input;
        KeyboardEvent keyboard_input;

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

        Window(const Window&) = delete;
        Window operator=(const Window&) = delete;
    };

}