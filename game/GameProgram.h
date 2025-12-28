//
// Created by Admin on 23/12/2025.
//

#pragma once
#include <../heaven_engine/GameObject.h>
#include <bits/unique_ptr.h>
#include <../heaven_engine/graphics/models/BasicModel.h>
#include <../heaven_engine/graphics/HeavenRenderer.h>

#include "../heaven_engine/HeavenWindow.h"

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

        HeavenWindow gameWindow;
        HeavenVkDevice vulkanDevice{gameWindow};
        HeavenRenderer renderer{gameWindow, vulkanDevice};

        std::vector<GameObject> gameObjects;
    };
}
