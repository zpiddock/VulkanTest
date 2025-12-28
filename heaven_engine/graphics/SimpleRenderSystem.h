//
// Created by Admin on 23/12/2025.
//

#pragma once
#include <GameObject.h>
#include <bits/unique_ptr.h>
#include <graphics/ShaderPipeline.h>
#include <graphics/models/BasicModel.h>

namespace heaven_engine {
    class SimpleRenderSystem {

    public:

        SimpleRenderSystem(VulkanDevice& device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

        auto renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjs) -> void;

    private:
        auto createPipeline(VkRenderPass renderPass) -> void;
        auto createPipelineLayout() -> void;

        VulkanDevice& vulkanDevice;
        std::unique_ptr<ShaderPipeline> shaderPipeline;

        VkPipelineLayout pipelineLayout;
    };
}
