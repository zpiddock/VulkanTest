//
// Created by Admin on 28/12/2025.
//

#pragma once
#include "GameObject.h"
#include "GLFW/glfw3.h"

namespace heaven_engine {
    class Keyboard_Movement_Controller {

    public:
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

        void moveInPlaneXZ(GLFWwindow* window, float deltaTime, GameObject& gameObject);

        KeyMappings keys{};
        float movementSpeed = 3.0f;
        float lookSpeed = 1.5f;
    };
} // heaven_engine