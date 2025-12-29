//
// Created by Admin on 28/12/2025.
//

#pragma once
#include "GameObject.h"
#include "GLFW/glfw3.h"
#include "game/GameConfigInfo.h"

namespace heaven_engine {

    class Movement_Controller {

    public:

        struct Callback_Context {
            GameConfigInfo& config;
            GameObject& viewerObject; // what we traditionally call the "camera"
            Movement_Controller& controller;
        };

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

        static void keyInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        static void mouseInputCallback(GLFWwindow * window, double xpos, double ypos);

        KeyMappings keys{};
        float movementSpeed = 3.0f;
        float lookSpeed = 1.5f;

        double lastMouseX = 0.0, lastMouseY;
        bool firstMouse = true;
    };
} // heaven_engine