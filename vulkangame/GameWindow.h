//
// Created by Admin on 22/12/2025.
//

#ifndef VULKANTEST_GAMEWINDOW_H
#define VULKANTEST_GAMEWINDOW_H
#include "GLFW/glfw3.h"

namespace vulkangame {


    class GameWindow {

        public:
        static GameWindow createInstance();

        int createWindow();
        void destroy();
        GLFWwindow* getWindowPtr();
    };
} // vulkangame

#endif //VULKANTEST_GAMEWINDOW_H