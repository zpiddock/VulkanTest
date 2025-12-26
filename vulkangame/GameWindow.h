//
// Created by Admin on 22/12/2025.
//

#pragma once

#include <vulkan/vulkan_core.h>

#include "GLFW/glfw3.h"

namespace vulkangame {

    class GameWindow {

    public:
        GameWindow();
        ~GameWindow();

        GameWindow(const GameWindow&) = delete;
        GameWindow &operator=(const GameWindow&) = delete;

        auto shouldClose() -> bool { return ::glfwWindowShouldClose(window); }

        auto createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) -> void;

        auto getWindowPtr() -> GLFWwindow*;

    private:
        GLFWwindow* window = nullptr;

        auto createWindow() -> int;

        auto destroy() -> void;
    };
} // vulkangame