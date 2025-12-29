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

    void Movement_Controller::keyInputCallback(GLFWwindow *window, int key, int scancode, int action,
        int mods) {

        auto context = reinterpret_cast<Callback_Context*>(::glfwGetWindowUserPointer(window));

        ::ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {

            context->config.imguiShouldOpen = !context->config.imguiShouldOpen;

            if (context->config.imguiShouldOpen) {
                ::glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                ::glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
    }

    void Movement_Controller::mouseInputCallback(GLFWwindow *window, double xpos, double ypos) {

        auto context = reinterpret_cast<Callback_Context*>(::glfwGetWindowUserPointer(window));

        ::ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

        // Don't rotate if ImGui is open
        if (context->config.imguiShouldOpen) {
            context->controller.firstMouse = true;
            return;
        }

        if (context->controller.firstMouse) {
            context->controller.lastMouseX = xpos;
            context->controller.lastMouseY = ypos;
            context->controller.firstMouse = false;
        }

        float xoffset = static_cast<float>(xpos - context->controller.lastMouseX);
        float yoffset = static_cast<float>(context->controller.lastMouseY - ypos); // Reversed: y-coords go from bottom to top

        context->controller.lastMouseX = xpos;
        context->controller.lastMouseY = ypos;

        float sensitivity = 0.002f; // Adjust this to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        // Apply rotation to the GameObject
        context->viewerObject.transform.rotation.y += xoffset;
        context->viewerObject.transform.rotation.x += yoffset;

        // Limit pitch so the camera doesn't flip upside down
        context->viewerObject.transform.rotation.x = glm::clamp(context->viewerObject.transform.rotation.x, -1.5f, 1.5f);

        // Limit yaw from growing too high if we spin in the same direction
        context->viewerObject.transform.rotation.y = glm::mod(context->viewerObject.transform.rotation.y, glm::two_pi<float>());
    }
} // heaven_engine