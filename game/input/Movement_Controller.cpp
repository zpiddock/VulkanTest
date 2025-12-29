//
// Created by Admin on 28/12/2025.
//

#include "Movement_Controller.h"

#include "imgui_impl_glfw.h"
#include "game/GameConfigInfo.h"
#include "glm/gtc/quaternion.hpp"

namespace heaven_engine {

    auto Movement_Controller::moveInPlaneXZ(GLFWwindow *window, float deltaTime, GameObject &gameObject) -> void {

        float yaw = gameObject.transform.rotation.y;
        const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
        const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
        const glm::vec3 upDir{0.f, 1.f, 0.f};

        glm::vec3 moveDir{0.f};

        if (::glfwGetKey(window, keys.moveForward) == GLFW_PRESS) {
            moveDir += forwardDir;
        }
        if (::glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) {
            moveDir -= forwardDir;
        }
        if (::glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) {
            moveDir -= rightDir;
        }
        if (::glfwGetKey(window, keys.moveRight) == GLFW_PRESS) {
            moveDir += rightDir;
        }
        if (::glfwGetKey(window, keys.moveUp) == GLFW_PRESS) {
            moveDir -= upDir;
        }
        if (::glfwGetKey(window, keys.moveDown) == GLFW_PRESS) {
            moveDir += upDir;
        }
        if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {

            gameObject.transform.translation += movementSpeed * deltaTime * glm::normalize(moveDir);
        }
    }

    void Movement_Controller::onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {

            config.imguiShouldOpen = !config.imguiShouldOpen;

            if (config.imguiShouldOpen) {
                ::glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                ::glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
    }

    void Movement_Controller::onMouseMove(GLFWwindow* window, double xpos, double ypos) {

        // Don't rotate if ImGui is open
        if (config.imguiShouldOpen) {
            firstMouse = true;
            return;
        }

        if (firstMouse) {
            lastMouseX = xpos;
            lastMouseY = ypos;
            firstMouse = false;
        }

        float xoffset = static_cast<float>(xpos - lastMouseX);
        float yoffset = static_cast<float>(lastMouseY - ypos); // Reversed: y-coords go from bottom to top

        lastMouseX = xpos;
        lastMouseY = ypos;

        float sensitivity = 0.002f; // Adjust this to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        // Apply rotation to the GameObject
        viewerObject->transform.rotation.y += xoffset;
        viewerObject->transform.rotation.x += yoffset;

        // Limit pitch so the camera doesn't flip upside down
        viewerObject->transform.rotation.x = glm::clamp(viewerObject->transform.rotation.x, -1.5f, 1.5f);

        // Limit yaw from growing too high if we spin in the same direction
        viewerObject->transform.rotation.y = glm::mod(viewerObject->transform.rotation.y, glm::two_pi<float>());
    }
} // heaven_engine