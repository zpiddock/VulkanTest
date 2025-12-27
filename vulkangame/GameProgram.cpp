//
// Created by Admin on 23/12/2025.
//

#include "GameProgram.h"

#include <exception>
#include <iostream>
#include <array>

namespace vulkangame {

    GameProgram::GameProgram() {

        loadModels();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    GameProgram::~GameProgram() {

        ::vkDestroyPipelineLayout(vulkanDevice.device(), pipelineLayout, nullptr);
    }

    auto GameProgram::loadModels() -> void {

        std::vector<BasicModel::Vertex> vertices = {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        };

        model = std::make_unique<BasicModel>(vulkanDevice, vertices);
    }

    auto GameProgram::createPipelineLayout() -> void {

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount = 0;
        pipelineLayoutCreateInfo.pSetLayouts = nullptr;
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
        clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
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
        model->bind(commandBuffers[imageIndex]);
        model->begin(commandBuffers[imageIndex]);

        ::vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (::vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer!");
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