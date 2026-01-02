//
// Created by Admin on 23/12/2025.
//

#pragma once
#include <../heaven_engine/GameObject.h>
#include <bits/unique_ptr.h>
#include <../heaven_engine/graphics/models/BasicModel.h>
#include <../heaven_engine/graphics/HeavenRenderer.h>

#include "../heaven_engine/HeavenWindow.h"
#include "graphics/HeavenDescriptors.h"

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


        // The order of declarations matters here! due to smart pointers, fields will get destroyed in reverse order.

        HeavenWindow gameWindow;
        HeavenVkDevice vulkanDevice{gameWindow};
        HeavenRenderer renderer{gameWindow, vulkanDevice};
        std::unique_ptr<HvnDescriptorPool> globalDescriptorPool{};

        GameObject::Map gameObjects;
    };
}
