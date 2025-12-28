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
        glm::mat2 transform{1.f};
        glm::vec2 offset;
        alignas(16)glm::vec3 colour;
    };

    SimpleRenderSystem::SimpleRenderSystem(VulkanDevice& device, VkRenderPass renderPass) : vulkanDevice{device} {

        createPipelineLayout();
        createPipeline(renderPass);
    }

    SimpleRenderSystem::~SimpleRenderSystem() {

        ::vkDestroyPipelineLayout(vulkanDevice.device(), pipelineLayout, nullptr);
    }

    auto SimpleRenderSystem::createPipelineLayout() -> void {

        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount = 0;
        pipelineLayoutCreateInfo.pSetLayouts = nullptr;
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
        ShaderPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;

        shaderPipeline = std::make_unique<ShaderPipeline>(
            vulkanDevice,
            "simple_shader",
            pipelineConfig);
    }

    auto SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjs) -> void {
        shaderPipeline->bind(commandBuffer);

        for (auto& obj : gameObjs) {

            obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());

            SimplePushConstantData push{};
            push.offset = obj.transform2d.translation;
            push.colour = obj.colour;
            push.transform = obj.transform2d.mat2();

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