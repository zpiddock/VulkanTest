//
// Created by Admin on 22/12/2025.
//

#pragma once

#include <vulkan/vulkan_core.h>

#include "GLFW/glfw3.h"

namespace heaven_engine {

    class GameWindow {

    public:
        GameWindow();
        ~GameWindow();

        GameWindow(const GameWindow&) = delete;
        GameWindow &operator=(const GameWindow&) = delete;

        auto shouldClose() -> bool { return ::glfwWindowShouldClose(window); }

        auto getExtent() -> VkExtent2D { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; };

        auto wasWindowResized() -> bool { return framebufferResized; };

        auto resetWindowResizedFlag() -> void { framebufferResized = false; };

        auto createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) -> void;

        auto getWindowPtr() -> GLFWwindow*;

    private:

        int width, height;

        bool framebufferResized = false;

        GLFWwindow* window = nullptr;

        static auto framebufferResizeCallback(GLFWwindow* window, int width, int height) -> void;

        auto createWindow() -> int;

        auto destroy() -> void;
    };
} // vulkangame