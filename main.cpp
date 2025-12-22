#define GLFW_INCLUDE_VULKAN

#include "vulkangame/GameWindow.h"

namespace vulkangame {
}

int main() {

    auto gameWindow = vulkangame::GameWindow::createInstance();

    gameWindow.createWindow();

    //Gameloop
    while(!glfwWindowShouldClose(gameWindow.getWindowPtr())) {
        glfwPollEvents();
    }

    gameWindow.destroy();

    return 0;
}