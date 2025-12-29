//
// Created by Admin on 22/12/2025.
//

#include "HeavenWindow.h"

#include <cstdio>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "GLFW/glfw3.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <stdexcept>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "InputManager.h"
#include "game/input/Movement_Controller.h"

namespace heaven_engine {

    HeavenWindow::HeavenWindow() {

        width = 800;
        height = 600;
        createWindow();
    }

    HeavenWindow::~HeavenWindow() {

        destroy();
    }

    auto HeavenWindow::createWindow() -> int {

        ::glfwInit();
        ::glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        ::glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        ::glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        window = ::glfwCreateWindow(width, height, "heaven_engine", nullptr, nullptr);

        if (window == nullptr) {

            throw std::runtime_error("Failed to create GLFW window!");
        }

        // ::glfwSetWindowUserPointer(window, this);
        ::glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

        return EXIT_SUCCESS;
    }

    auto HeavenWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR*surface) -> void {

        if (::glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {

            throw std::runtime_error("Failed to create GLFW window surface!");
        }
    }

    auto HeavenWindow::destroy() -> void {

        glfwDestroyWindow(window);

        glfwTerminate();
    }

    auto HeavenWindow::getWindowPtr() -> GLFWwindow* {

        return window;
    }

    auto HeavenWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) -> void {

        auto context = reinterpret_cast<InputManager::WindowContext*>(::glfwGetWindowUserPointer(window));
        context->heavenWindow.framebufferResized = true;
        context->heavenWindow.width = width;
        context->heavenWindow.height = height;
    }
}
