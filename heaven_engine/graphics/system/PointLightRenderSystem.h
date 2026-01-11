//
// Created by Admin on 23/12/2025.
//

#pragma once
#include <GameObject.h>
#include <bits/unique_ptr.h>
#include <graphics/HeavenShaderPipeline.h>
#include <graphics/models/BasicModel.h>

#include "../HeavenFrameInfo.h"
#include "../HvnCamera.h"

namespace heaven_engine {
    class PointLightRenderSystem {

    public:

        PointLightRenderSystem(HeavenVkDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~PointLightRenderSystem();

        PointLightRenderSystem(const PointLightRenderSystem&) = delete;
        PointLightRenderSystem& operator=(const PointLightRenderSystem&) = delete;

        auto update(FrameInfo &frameInfo, GlobalUbo &ubo) -> void;
        auto render(FrameInfo &frameInfo) -> void;

    private:
        auto createPipeline(VkRenderPass renderPass) -> void;
        auto createPipelineLayout(VkDescriptorSetLayout globalSetLayout) -> void;

        HeavenVkDevice& vulkanDevice;
        std::unique_ptr<HeavenShaderPipeline> shaderPipeline;

        VkPipelineLayout pipelineLayout;
    };
}
