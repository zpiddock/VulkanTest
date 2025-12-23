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

    GameWindow::GameWindow() {

        createWindow();
    }

    GameWindow::~GameWindow() {

        destroy();
    }

    auto GameWindow::createWindow() -> int {

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(800, 600, "vulkangame", nullptr, nullptr);

        if (window == nullptr) {

            throw std::runtime_error("Failed to create GLFW window!");
        }

        return EXIT_SUCCESS;
    }

    auto GameWindow::destroy() -> void {

        glfwDestroyWindow(window);

        glfwTerminate();
    }

    auto GameWindow::getWindowPtr() -> GLFWwindow* {

        return window;
    }
}
