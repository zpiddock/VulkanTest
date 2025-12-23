//
// Created by Admin on 23/12/2025.
//

#pragma once
#include <graphics/ShaderProgram.h>

#include "GameWindow.h"

namespace vulkangame {
    class GameProgram {

    public:

        auto run() -> void;

    private:
        GameWindow gameWindow;
        ShaderProgram simple_shader{"simple_shader"};
    };
}
