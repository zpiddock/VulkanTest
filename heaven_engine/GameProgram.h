//
// Created by Admin on 23/12/2025.
//

#pragma once
#include <GameObject.h>
#include <bits/unique_ptr.h>
#include <graphics/models/BasicModel.h>
#include <graphics/HeavenRenderer.h>

#include "GameWindow.h"

namespace heaven_engine {
    class GameProgram {

    public:

        GameProgram();
        ~GameProgram();

        GameProgram(const GameProgram&) = delete;
        GameProgram& operator=(const GameProgram&) = delete;

        auto run() -> void;

    private:
        auto loadObjects() -> void;

        GameWindow gameWindow;
        VulkanDevice vulkanDevice{gameWindow};
        HeavenRenderer renderer{gameWindow, vulkanDevice};

        std::vector<GameObject> gameObjects;
    };
}
