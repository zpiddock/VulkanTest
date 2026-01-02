//
// Created by Admin on 23/12/2025.
//

#include "SimpleRenderSystem.h"

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

    SimpleRenderSystem::SimpleRenderSystem(HeavenVkDevice &device, VkRenderPass renderPass,
                                           VkDescriptorSetLayout globalSetLayout) : vulkanDevice{device} {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }

    SimpleRenderSystem::~SimpleRenderSystem() {
        ::vkDestroyPipelineLayout(vulkanDevice.device(), pipelineLayout, nullptr);
    }

    auto SimpleRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) -> void {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        std::vector setLayouts{globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
        pipelineLayoutCreateInfo.pSetLayouts = setLayouts.data();
        pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
        pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

        if (::vkCreatePipelineLayout(
                vulkanDevice.device(),
                &pipelineLayoutCreateInfo,
                nullptr,
                &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout!");
        }
    }

    auto SimpleRenderSystem::createPipeline(VkRenderPass renderPass) -> void {
        PipelineConfigInfo pipelineConfig{};
        HeavenShaderPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;

        shaderPipeline = std::make_unique<HeavenShaderPipeline>(
            vulkanDevice,
            "simple_shader",
            pipelineConfig);
    }

    auto SimpleRenderSystem::renderGameObjects(FrameInfo &frameInfo) -> void {
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

        for (auto &kvPair: frameInfo.gameObjects) {
            auto &obj = kvPair.second;
            if (obj.model == nullptr) continue;
            SimplePushConstantData push{};
            push.modelMatrix = obj.transform.mat4();
            push.normalMatrix = obj.transform.normalMatrix();

            ::vkCmdPushConstants(commandBuffer,
                                 pipelineLayout,
                                 VK_SHADER_STAGE_VERTEX_BIT |
                                 VK_SHADER_STAGE_FRAGMENT_BIT,
                                 0,
                                 sizeof(SimplePushConstantData),
                                 &push);
            obj.model->bind(commandBuffer);
            obj.model->begin(commandBuffer);
        }
    }
} // heaven_engine
