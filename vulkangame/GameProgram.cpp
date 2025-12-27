//
// Created by Admin on 23/12/2025.
//

#include "GameProgram.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <GameObject.h>
#include <iostream>
#include <array>
#include <cmake-build-release/_deps/glm-src/glm/gtc/constants.hpp>
#include <glm/ext/scalar_constants.hpp>

namespace vulkangame {

    struct SimplePushConstantData {
        glm::mat2 transform{1.f};
        glm::vec2 offset;
        alignas(16)glm::vec3 colour;
    };

    GameProgram::GameProgram() {

        loadObjects();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    GameProgram::~GameProgram() {

        ::vkDestroyPipelineLayout(vulkanDevice.device(), pipelineLayout, nullptr);
    }

    auto GameProgram::loadObjects() -> void {

        std::vector<BasicModel::Vertex> vertices = {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        };

        auto model = std::make_shared<BasicModel>(vulkanDevice, vertices);

        auto triangle = GameObject::createGameObject();
        triangle.model = model;
        triangle.colour = {0.1f, 0.8f, 0.1f};
        triangle.transform2d.translation.x = 0.2f;
        triangle.transform2d.scale = {2.f, 0.5f};
        triangle.transform2d.rotation = 0.25f * glm::two_pi<float>();

        gameObjects.push_back(std::move(triangle));
    }

    auto GameProgram::createPipelineLayout() -> void {

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

    auto GameProgram::createPipeline() -> void {

        PipelineConfigInfo pipelineConfig{};
        ShaderPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = vulkanSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;

        shaderPipeline = std::make_unique<ShaderPipeline>(
            vulkanDevice,
            "simple_shader",
            pipelineConfig);
    }

    auto GameProgram::createCommandBuffers() -> void {

        commandBuffers.resize(vulkanSwapChain->imageCount());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = vulkanDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (::vkAllocateCommandBuffers(
            vulkanDevice.device(),
            &allocInfo,
            commandBuffers.data()
            ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffers!");
        }
    }

    auto GameProgram::freeCommandBuffers() -> void {

        ::vkFreeCommandBuffers(vulkanDevice.device(), vulkanDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }

    auto GameProgram::recreateSwapChain() -> void {

        auto extent = gameWindow.getExtent();
        while (extent.width == 0 || extent.height == 0) {

            extent = gameWindow.getExtent();
            ::glfwWaitEvents();
        }

        ::vkDeviceWaitIdle(vulkanDevice.device());
        // vulkanSwapChain = nullptr;

        if (vulkanSwapChain == nullptr) {
            vulkanSwapChain = std::make_unique<VulkanSwapChain>(vulkanDevice, extent);
        } else {
            vulkanSwapChain = std::make_unique<VulkanSwapChain>(vulkanDevice, extent, std::move(vulkanSwapChain));
            if (vulkanSwapChain->imageCount() != commandBuffers.size()) {
                freeCommandBuffers();
                createCommandBuffers();
            }
        }

        // if render pass is compatible, do nothing
        createPipeline();
    }

    auto GameProgram::recordCommandBuffer(int imageIndex) -> void {

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (::vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin command buffer!");
        }

        VkRenderPassBeginInfo renderPassBeginInfo{};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = vulkanSwapChain->getRenderPass();
        renderPassBeginInfo.framebuffer = vulkanSwapChain->getFrameBuffer(imageIndex);

        renderPassBeginInfo.renderArea.offset = { 0, 0 };
        renderPassBeginInfo.renderArea.extent = vulkanSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassBeginInfo.pClearValues = clearValues.data();

        ::vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(vulkanSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(vulkanSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, vulkanSwapChain->getSwapChainExtent()};
        ::vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        ::vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        shaderPipeline->bind(commandBuffers[imageIndex]);

        rendergameObjects(commandBuffers[imageIndex]);

        ::vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (::vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer!");
        }
    }

    auto GameProgram::rendergameObjects(VkCommandBuffer commandBuffer) -> void {

        shaderPipeline->bind(commandBuffer);

        for (auto& obj : gameObjects) {

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

    auto GameProgram::drawFrame() -> void {

        uint32_t imageIndex;
        auto result = vulkanSwapChain->acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }

        recordCommandBuffer(imageIndex);
        result = vulkanSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || gameWindow.wasWindowResized()) {
            gameWindow.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to submit command buffer!");
        }
    }

    auto GameProgram::run() -> void {

        //Gameloop
        while(!gameWindow.shouldClose()) {
            ::glfwPollEvents();
            drawFrame();
        }

        ::vkDeviceWaitIdle(vulkanDevice.device());
    }
} // vulkangame