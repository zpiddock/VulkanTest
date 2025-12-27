//
// Created by Admin on 23/12/2025.
//

#pragma once
#include <GameObject.h>
#include <bits/unique_ptr.h>
#include <graphics/ShaderPipeline.h>
#include <graphics/VulkanSwapChain.h>
#include <graphics/models/BasicModel.h>

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
        auto createPipeline() -> void;
        auto createPipelineLayout() -> void;
        auto createCommandBuffers() -> void;
        auto freeCommandBuffers() -> void;
        auto drawFrame() -> void;
        auto recreateSwapChain() -> void;
        auto recordCommandBuffer(int imageIndex) -> void;
        auto rendergameObjects(VkCommandBuffer commandBuffer) -> void;

        GameWindow gameWindow;
        VulkanDevice vulkanDevice{gameWindow};
        std::unique_ptr<VulkanSwapChain> vulkanSwapChain;
        std::unique_ptr<ShaderPipeline> shaderPipeline;

        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;

        std::vector<GameObject> gameObjects;
    };
}
