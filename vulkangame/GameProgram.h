//
// Created by Admin on 23/12/2025.
//

#pragma once
#include <bits/unique_ptr.h>
#include <graphics/ShaderPipeline.h>
#include <graphics/VulkanSwapChain.h>

#include "GameWindow.h"

namespace vulkangame {
    class GameProgram {

    public:

        GameProgram();
        ~GameProgram();

        GameProgram(const GameProgram&) = delete;
        GameProgram& operator=(const GameProgram&) = delete;

        auto run() -> void;

    private:
        auto createPipeline() -> void;
        auto createPipelineLayout() -> void;
        auto createCommandBuffers() -> void;
        auto drawFrame() -> void;

        GameWindow gameWindow;
        VulkanDevice vulkanDevice{gameWindow};
        VulkanSwapChain vulkanSwapChain{vulkanDevice, gameWindow.getExtent()};

        std::unique_ptr<ShaderPipeline> shaderPipeline;

        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;

        // ShaderPipeline simple_shader{
        //     vulkanDevice,
        //     "simple_shader",
        //     ShaderPipeline::defaultPipelineConfigInfo(800, 600)
        // };
    };
}
