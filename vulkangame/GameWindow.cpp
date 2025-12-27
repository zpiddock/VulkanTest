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

        width = 800;
        height = 600;
        createWindow();
    }

    GameWindow::~GameWindow() {

        destroy();
    }

    auto GameWindow::createWindow() -> int {

        ::glfwInit();
        ::glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        ::glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        window = ::glfwCreateWindow(width, height, "vulkangame", nullptr, nullptr);

        if (window == nullptr) {

            throw std::runtime_error("Failed to create GLFW window!");
        }

        ::glfwSetWindowUserPointer(window, this);
        ::glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

        return EXIT_SUCCESS;
    }

    auto GameWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR*surface) -> void {

        if (::glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {

            throw std::runtime_error("Failed to create GLFW window surface!");
        }
    }

    auto GameWindow::destroy() -> void {

        glfwDestroyWindow(window);

        glfwTerminate();
    }

    auto GameWindow::getWindowPtr() -> GLFWwindow* {

        return window;
    }

    auto GameWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) -> void {

        auto gameWindow = reinterpret_cast<GameWindow*>(::glfwGetWindowUserPointer(window));
        gameWindow->framebufferResized = true;
        gameWindow->width = width;
        gameWindow->height = height;
    }
}
