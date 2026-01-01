//
// Created by Admin on 23/12/2025.
//

#pragma once
#include <GameObject.h>
#include <bits/unique_ptr.h>
#include <graphics/HeavenShaderPipeline.h>
#include <graphics/models/BasicModel.h>

#include "HeavenFrameInfo.h"
#include "HvnCamera.h"

namespace heaven_engine {
    class SimpleRenderSystem {

    public:

        SimpleRenderSystem(HeavenVkDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

        auto renderGameObjects(FrameInfo &frameInfo, std::vector<GameObject>& gameObjs) -> void;

    private:
        auto createPipeline(VkRenderPass renderPass) -> void;
        auto createPipelineLayout(VkDescriptorSetLayout globalSetLayout) -> void;

        HeavenVkDevice& vulkanDevice;
        std::unique_ptr<HeavenShaderPipeline> shaderPipeline;

        VkPipelineLayout pipelineLayout;
    };
}
