//
// Created by Admin on 28/12/2025.
//

#pragma once
#include "GameObject.h"
#include "InputManager.h"
#include "GLFW/glfw3.h"
#include "game/GameConfigInfo.h"

namespace heaven_engine {

    class Movement_Controller : public IInputSubscriber {

    public:

        Movement_Controller(GameConfigInfo& config) : config{config} {};

        ~Movement_Controller() = default;

        struct KeyMappings {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_SPACE;
            int moveDown = GLFW_KEY_LEFT_SHIFT;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
        };

        auto moveInPlaneXZ(GLFWwindow* window, float deltaTime, GameObject& gameObject) -> void;

        // Implement the interface
        void onKey(GLFWwindow* window, int key, int scancode, int action, int mods) override;
        void onMouseMove(GLFWwindow* window, double xpos, double ypos) override;

        // We need a reference to the object we want to rotate
        void setTargetObject(GameObject& gameObject) { viewerObject = &gameObject; }

        KeyMappings keys{};
        float movementSpeed = 3.0f;
        float lookSpeed = 1.5f;

        double lastMouseX = 0.0, lastMouseY = 0.0;
        bool firstMouse = true;

    private:
        GameConfigInfo& config;
        GameObject* viewerObject = nullptr;
    };
} // heaven_engine