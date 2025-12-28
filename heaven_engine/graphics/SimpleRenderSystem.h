//
// Created by Admin on 23/12/2025.
//

#pragma once
#include <GameObject.h>
#include <bits/unique_ptr.h>
#include <graphics/HeavenShaderPipeline.h>
#include <graphics/models/BasicModel.h>

#include "HvnCamera.h"

namespace heaven_engine {
    class SimpleRenderSystem {

    public:

        SimpleRenderSystem(HeavenVkDevice& device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

        auto renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjs, const HvnCamera& camera) -> void;

    private:
        auto createPipeline(VkRenderPass renderPass) -> void;
        auto createPipelineLayout() -> void;

        HeavenVkDevice& vulkanDevice;
        std::unique_ptr<HeavenShaderPipeline> shaderPipeline;

        VkPipelineLayout pipelineLayout;
    };
}
