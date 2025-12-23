//
// Created by Admin on 23/12/2025.
//

#include "GameProgram.h"

#include <exception>
#include <iostream>

namespace vulkangame {

    auto GameProgram::run() -> void {

        //Gameloop
        while(!gameWindow.shouldClose()) {
            ::glfwPollEvents();
        }
    }
} // vulkangame