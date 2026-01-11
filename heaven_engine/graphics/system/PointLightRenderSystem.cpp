//
// Created by Admin on 23/12/2025.
//

#include "PointLightRenderSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <GameObject.h>
#include <iostream>

#include <glm/gtc/constants.hpp>

namespace heaven_engine {
    struct SimplePushConstantData {
        glm::mat4 modelMatrix{1.f};
        glm::mat4 normalMatrix{1.f};
    };

    PointLightRenderSystem::PointLightRenderSystem(HeavenVkDevice &device, VkRenderPass renderPass,
                                           VkDescriptorSetLayout globalSetLayout) : vulkanDevice{device} {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }

    PointLightRenderSystem::~PointLightRenderSystem() {
        ::vkDestroyPipelineLayout(vulkanDevice.device(), pipelineLayout, nullptr);
    }

    auto PointLightRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) -> void {
        // VkPushConstantRange pushConstantRange{};
        // pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        // pushConstantRange.offset = 0;
        // pushConstantRange.size = sizeof(SimplePushConstantData);

        std::vector setLayouts{globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
        pipelineLayoutCreateInfo.pSetLayouts = setLayouts.data();
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

        if (::vkCreatePipelineLayout(
                vulkanDevice.device(),
                &pipelineLayoutCreateInfo,
                nullptr,
                &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout!");
        }
    }

    auto PointLightRenderSystem::createPipeline(VkRenderPass renderPass) -> void {
        PipelineConfigInfo pipelineConfig{};
        HeavenShaderPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.vertexInputBindingDescriptions.clear();
        pipelineConfig.vertexInputAttributeDescriptions.clear();
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;

        shaderPipeline = std::make_unique<HeavenShaderPipeline>(
            vulkanDevice,
            "pointlight",
            pipelineConfig);
    }

    auto PointLightRenderSystem::render(FrameInfo &frameInfo) -> void {
        auto commandBuffer = frameInfo.commandBuffer;

        shaderPipeline->bind(commandBuffer);

        ::vkCmdBindDescriptorSets(commandBuffer,
                                  VK_PIPELINE_BIND_POINT_GRAPHICS,
                                  pipelineLayout,
                                  0,
                                  1,
                                  &frameInfo.globalDescriptorSet,
                                  0,
                                  nullptr);

        //draw point light
        ::vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    }
} // heaven_engine
