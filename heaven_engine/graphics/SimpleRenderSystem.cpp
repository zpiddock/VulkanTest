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
        glm::mat4 transform{1.f};
        alignas(16)glm::vec3 colour;
    };

    SimpleRenderSystem::SimpleRenderSystem(HeavenVkDevice& device, VkRenderPass renderPass) : vulkanDevice{device} {

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
        HeavenShaderPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;

        shaderPipeline = std::make_unique<HeavenShaderPipeline>(
            vulkanDevice,
            "simple_shader",
            pipelineConfig);
    }

    auto SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjs, const HvnCamera& camera) -> void {
        shaderPipeline->bind(commandBuffer);

        for (auto& obj : gameObjs) {

            obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.01f, glm::two_pi<float>());
            obj.transform.rotation.z = glm::mod(obj.transform.rotation.z + 0.01f, glm::two_pi<float>());

            SimplePushConstantData push{};
            push.colour = obj.colour;
            push.transform = camera.getProjectionMatrix() * obj.transform.mat4();

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