//
// Created by Admin on 23/12/2025.
//

#include "PointLightRenderSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <GameObject.h>
#include <iostream>

namespace heaven_engine {
    struct PointLightPushConstants {
        glm::vec4 position{};
        glm::vec4 color{};
        float radius{};
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
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PointLightPushConstants);

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

    auto PointLightRenderSystem::update(FrameInfo &frameInfo, GlobalUbo &ubo) -> void {

        auto rotate = glm::rotate(glm::mat4(1.f), frameInfo.frameTime, glm::vec3(0.f, 1.f, 0.f));

        int lightIndex = 0;
        for (auto &kvPair : frameInfo.gameObjects) {
            auto &gameObject = kvPair.second;
            if (gameObject.pointLight == nullptr) continue;

            //update light translation
            gameObject.transform.translation = glm::vec3(rotate * glm::vec4(gameObject.transform.translation, 1.f));

            ubo.pointLights[lightIndex].position = glm::vec4(gameObject.transform.translation, 1.f);
            ubo.pointLights[lightIndex].colour = glm::vec4(gameObject.colour, gameObject.pointLight->lightIntensity);
            lightIndex += 1;
        }
        ubo.numLights = lightIndex;
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

        for (auto &kvPair : frameInfo.gameObjects) {
            auto &gameObject = kvPair.second;
            if (gameObject.pointLight == nullptr) continue;

            PointLightPushConstants pushConstants{};
            pushConstants.position = glm::vec4(gameObject.transform.translation, 1.f);
            pushConstants.color = glm::vec4(gameObject.colour, gameObject.pointLight->lightIntensity);
            pushConstants.radius = gameObject.transform.scale.x;

            ::vkCmdPushConstants(commandBuffer,
                                 pipelineLayout,
                                 VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                                 0,
                                 sizeof(PointLightPushConstants),
                                 &pushConstants);
            //draw point light
            ::vkCmdDraw(commandBuffer, 6, 1, 0, 0);
        }

    }
} // heaven_engine
