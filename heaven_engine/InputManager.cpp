#include "InputManager.h"
#include <imgui_impl_glfw.h>

namespace heaven_engine {

    InputManager::InputManager(HeavenWindow& window) : window(window), context{window, *this} {
        ::glfwSetWindowUserPointer(window.getWindowPtr(), &context);
        ::glfwSetKeyCallback(window.getWindowPtr(), keyCallback);
        ::glfwSetCursorPosCallback(window.getWindowPtr(), cursorCallback);
    }

    InputManager::~InputManager() {
        ::glfwSetKeyCallback(window.getWindowPtr(), nullptr);
        ::glfwSetCursorPosCallback(window.getWindowPtr(), nullptr);
    }

    void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* ctx = reinterpret_cast<WindowContext*>(::glfwGetWindowUserPointer(window));
        
        // 1. Notify ImGui
        ::ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
        
        // 2. Notify Subscribers (like Movement_Controller)
        for (auto* s : ctx->inputManager.subscribers) {
            s->onKey(window, key, scancode, action, mods);
        }
    }

    void InputManager::cursorCallback(GLFWwindow* window, double xpos, double ypos) {
        auto* ctx = reinterpret_cast<WindowContext*>(::glfwGetWindowUserPointer(window));
        
        ::ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
        
        for (auto* s : ctx->inputManager.subscribers) {
            s->onMouseMove(window, xpos, ypos);
        }
    }
}