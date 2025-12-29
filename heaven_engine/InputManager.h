#pragma once

#include "HeavenWindow.h"
#include <GLFW/glfw3.h>
#include <functional>
#include <vector>

namespace heaven_engine {

    // Forward declaration of the interface that game-side controllers will implement
    class IInputSubscriber {
    public:
        virtual ~IInputSubscriber() = default;
        virtual void onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {}
        virtual void onMouseMove(GLFWwindow* window, double xpos, double ypos) {}
        virtual void onMouseButton(GLFWwindow* window, int button, int action, int mods) {}
        virtual void onScroll(GLFWwindow* window, double xoffset, double yoffset) {}
    };

    class InputManager {
    public:
        // This is the new "Bridge" struct that will be the User Pointer
        struct WindowContext {
            HeavenWindow& heavenWindow;
            InputManager& inputManager;
        };

        InputManager(HeavenWindow& window);
        ~InputManager();

        void addSubscriber(IInputSubscriber* subscriber) { subscribers.push_back(subscriber); }
        
        // Static callbacks for GLFW
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void cursorCallback(GLFWwindow* window, double xpos, double ypos);
        
    private:
        HeavenWindow& window;
        WindowContext context;
        std::vector<IInputSubscriber*> subscribers;
    };
}