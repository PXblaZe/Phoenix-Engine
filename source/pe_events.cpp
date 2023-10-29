#include "PXE/pe_init.hpp"
#include "PXE/pe_window.hpp"
#include "PXE/pe_events.hpp"

namespace px
{

    void Mouse::cpcall(GLFWwindow* win, double x, double y) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));

        if (!window->mouse_input.read_events) return;

        window->mouse_input.events.x = x, window->mouse_input.events.y = y;
        
        if (window->mouse_input.mouseFirstMoved)
        {
            window->mouse_input.lastx = window->mouse_input.events.x;
            window->mouse_input.lasty = window->mouse_input.events.y;
            window->mouse_input.mouseFirstMoved = false;
        }

        window->mouse_input.xChange = window->mouse_input.events.x - window->mouse_input.lastx;
        window->mouse_input.yChange = window->mouse_input.lasty - window->mouse_input.events.y;

        window->mouse_input.lastx = window->mouse_input.events.x;
        window->mouse_input.lasty = window->mouse_input.events.y;
    }

    void Mouse::cecall(GLFWwindow* win, int ent) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
        if (!window->mouse_input.read_events) return;
        window->mouse_input.events.isCursorEntered = ent ? true: false;
    }

    void Mouse::mbcall(GLFWwindow* win, int button, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
        if (!window->mouse_input.read_events) return;
        window->mouse_input.events.button = button, window->mouse_input.events.action = action, window->mouse_input.events.mods = mods;
    }

    void Keyboard::kbcall(GLFWwindow* win, int key, int scancode, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
        if (!window->keyboard_input.read_events) return;
        window->keyboard_input.events.key = key;
        window->keyboard_input.events.scancode = scancode;
        window->keyboard_input.events.action = action;
        window->keyboard_input.events.mods = mods;
        window->keyboard_input.events.key_action_record[key] = action;
    }



    double Mouse::getXChange() noexcept 
    {
        double theChange = xChange;
        xChange = 0.0f;
        return theChange;
    }
    
    
    double Mouse::getYChange() noexcept 
    {
        double theChange = yChange;
        yChange = 0.0f;
        return theChange;
    }

} // namespace px


    