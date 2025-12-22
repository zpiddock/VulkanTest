//
// Created by Admin on 22/12/2025.
//

#include "GameWindow.h"

#include <cstdio>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "GLFW/glfw3.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <stdexcept>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace vulkangame {

    GLFWwindow* window = nullptr;

    GameWindow GameWindow::createInstance() {
        return {};
    }

    int GameWindow::createWindow() {

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        window = glfwCreateWindow(800, 600, "vulkangame", nullptr, nullptr);

        if (window == nullptr) {

            throw std::runtime_error("failed to create GLFW window");
        }

        uint32_t extensionCount = 0;

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        return 0;
    }

    void GameWindow::destroy() {

        glfwDestroyWindow(window);

        glfwTerminate();
    }

    GLFWwindow* GameWindow::getWindowPtr() {

        return window;
    }
}
